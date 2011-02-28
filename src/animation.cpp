#include "animation.h"

// ----- Maya -----------------------------------------------------------------------
#include <maya/MPlug.h>

#include <maya/MFnClip.h>
#include <maya/MString.h>
#include <maya/MVector.h>
#include <maya/MFnIkJoint.h>
#include <maya/MDGModifier.h>
#include <maya/MQuaternion.h>
#include <maya/MFnCharacter.h>
#include <maya/MFnTransform.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>


// ----- OSG ------------------------------------------------------------------------
#include <osg/Material>
#include <osg/MatrixTransform>

#include <osgAnimation/Bone>
#include <osgAnimation/UpdateBone>
#include <osgAnimation/UpdateMaterial>
#include <osgAnimation/UpdateMatrixTransform>
#include <osgAnimation/AnimationUpdateCallback>

#include <osgAnimation/StackedScaleElement>
#include <osgAnimation/StackedTranslateElement>
#include <osgAnimation/StackedQuaternionElement>




// Static Data Member Definitions
bool				Animation::_isInited = false ;	// Setting Attributes once per Plug-In Load
double			Animation::_slopeTollerance ;		//	key if slope between keys changes more then this value
MStatus			Animation::_mStatus ;
MObjectArray	Animation::_mKeyframeNode ;		// holds all DAG Paths for key framing
MIntArray		Animation::_mKeyframeAttr ;		// holds the corresponding Pointer into keyAttributes ( Types, e.g. "translate" ) 
MStringArray	Animation::_keyAttributes ;		// Array of Attributes for keying
MDoubleArray	Animation::_keyDefaults ;			// Holds default Values for keyAttributes
MSpace::Space	Animation::_mQuatSpace ;

std::vector< std::string >	Animation::_osgAttributes ;	// Holds defining osg channel names
osg::ref_ptr< osgAnimation::BasicAnimationManager > Animation::_manager ;


void Animation::init()  {
	_manager = new osgAnimation::BasicAnimationManager ;

	_slopeTollerance = 0.0001 ;

	if ( !_isInited )  {
		_keyAttributes.append( "translate" ) ; 
		_keyAttributes.append( "rotate" ) ;
		_keyAttributes.append( "scale" ) ;
		_keyAttributes.append( "color" ) ;

		_keyDefaults.append( 0.0 ) ;
		_keyDefaults.append( 0.0 ) ;
		_keyDefaults.append( 1.0 ) ;
		_keyDefaults.append( 0.0 ) ;

		_osgAttributes.push_back( "position" ) ;
		_osgAttributes.push_back( "quaternion" ) ;
		_osgAttributes.push_back( "scale" ) ;
		_osgAttributes.push_back( "diffuse" ) ;

		_mQuatSpace = MSpace::kTransform ;

		_isInited = true ;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// - Map ------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
void Animation::mapInputConnections( const MObject & mObject , osg::ref_ptr< osg::MatrixTransform > & osgXForm )
{
	// To Do ---------------------------------------------
	// Do Not create Stacked Transform Elements for default values
	// Do Not create Keyframes for not keyed Stacked Transform Elements ( non Default Values )
	// - Is working properly for translate, but not for scale  ???
	// --- Check if hasInputConnections Flag stays for all Transform Elements

	bool hasInputs = false ;		// check for Inputs
	MFnDependencyNode	mfnDependencyNode( mObject ) ;
	MPlugArray mPlugInputArray ;	// will hold the connections to the current plug of this node
	MPlug mPlug ;


	// Find all Inputs to this DAG Path
	// Earlier looped over all Attributes in _keyAttributes with ka < _keyAttributes.length()
	// but now having more than only transform Attributes, loop only the first 3 entries
	for( unsigned int ka = 0 ; ka < 3 ; ++ka )	// ka = _keyAttributes
	{
		// get the MPlug for the transform attribute
		mPlug = mfnDependencyNode.findPlug( _keyAttributes[ ka ] , true , & _mStatus ) ;


		// If the Attribute is compound ( currently each is compound, e.g. tx,ty,tz ) check for child plugs
		if ( mPlug.isCompound() )  {
			for( unsigned int cp = 0 ; cp < mPlug.numChildren() ; ++cp )  {							// cp = childPlug
				mPlug.child( cp ).connectedTo( mPlugInputArray , true , false ) ;	
				hasInputs = hasInputs || ( mPlugInputArray.length() > 0 ) ;
			}
		}

		// else Attribute is not compound, still check for default value, even though not in use yet
		else  {
			mPlug.connectedTo( mPlugInputArray , true , false ) ;
			hasInputs = mPlugInputArray.length() > 0 ;
		}
		

		// hasInputs determines if the current Transform has Transformation Inputs of any kind
		// to map a Channel to the current Transform Type ( e.g. rotation ), query the length of mPlugInputArray
		if ( hasInputs )  {
			_mStatus = _mKeyframeNode.append( mObject ) ;
			_mStatus = _mKeyframeAttr.append( ka ) ;
		}
	}


	// If the Maya Object has no Input Connections, there is no need for an Animation UpdateCallback, but for a Joint
	if ( hasInputs || mObject.hasFn( MFn::kJoint ) )  {
		osgAnimation::UpdateMatrixTransform * updateCallback ;
		
		// Create Different Callbacks for Transforms and Joints ( second is derived from UpdateMatrixTransform )
		if ( mObject.hasFn( MFn::kJoint ) )
			updateCallback = new osgAnimation::UpdateBone ;

		else
			updateCallback = new osgAnimation::UpdateMatrixTransform ;

		MFnDagNode mfnDagNode( mObject , & _mStatus ) ;
		if ( _mStatus == MS::kSuccess )  {
			std::string callbackName( mfnDagNode.partialPathName( & _mStatus ).asChar() ) ;
			callbackName.append( "_Callback" ) ;
			updateCallback -> setName( callbackName ) ;
			osgXForm -> addUpdateCallback( updateCallback ) ;
			osgXForm -> setDataVariance( osg::Object::DYNAMIC ) ;

		}


		
		MFnTransform	mfnTransform( mObject ) ;	// attach the function set to the object
		osg::Vec3		osgVec3 ;
		osg::Quat		osgQuat ;

		// Order is important !!! 1.Translation, 2.Rotation, 3.Scale
		// 1.Translation
		MVector mVector = mfnTransform.getTranslation( MSpace::kTransform , & _mStatus ) ;
		osgVec3.set( osg::Vec3( mVector.x , mVector.y  , mVector.z )) ;
		updateCallback -> getStackedTransforms().push_back( new osgAnimation::StackedTranslateElement ( _osgAttributes[ 0 ] , osgVec3 ) ) ;

		//	2.Rotation
		MQuaternion	mQuat , mOrient ;
		_mStatus = mfnTransform.getRotation( mQuat , _mQuatSpace ) ;
		// If transform is a Joint, jointOrient must be multiplied with rotation Quaternion			
		if ( mObject.hasFn( MFn::kJoint ) )  {
			MFnIkJoint	mfnJoint( mObject , & _mStatus ) ;
			_mStatus = mfnJoint.getOrientation( mOrient ) ;
			mQuat = mQuat * mOrient ;
		}

		osgQuat.set( osg::Vec4f( mQuat.x , mQuat.y  , mQuat.z , mQuat.w )) ;
		updateCallback -> getStackedTransforms().push_back( new osgAnimation::StackedQuaternionElement( _osgAttributes[ 1 ] , osgQuat ) ) ;

		//	3.Scale
		double3			mScale ;
		_mStatus = mfnTransform.getScale( mScale ) ;
		osgVec3.set( osg::Vec3( mScale[ 0 ] , mScale[ 1 ]  , mScale[ 2 ] )) ;
		updateCallback -> getStackedTransforms().push_back( new osgAnimation::StackedScaleElement     ( _osgAttributes[ 2 ] , osgVec3 ) ) ;

	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// - Map Shader Input Connections to osg::Material ------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
void Animation::mapInputConnections( const MObject & mObject , osg::ref_ptr< osg::Material > & osgMaterial )
{
	// To Do ---------------------------------------------

	_mStatus = _mKeyframeNode.append( mObject ) ;
	_mStatus = _mKeyframeAttr.append( 3 ) ;

	MFnDependencyNode mfnShaderNode( mObject , & _mStatus ) ;
	std::string callbackName( mfnShaderNode.name( & _mStatus ).asChar() ) ;

	//logFile << "Mapping Maya Shader Attributes of \"" << callbackName << "\" to Color and Alpha of osg::Material" << std::endl ;

	callbackName.append( "_Callback" ) ;

	osg::ref_ptr< osgAnimation::UpdateMaterial > updateMaterial = new osgAnimation::UpdateMaterial ;

	updateMaterial -> setName( callbackName ) ;
	osgMaterial -> setUpdateCallback( updateMaterial ) ;
	osgMaterial -> setDataVariance( osg::Object::DYNAMIC ) ;
	
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// - Clip -----------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
void Animation::addAnimationClip( std::string clipName )
{
	std::string	targetName ; // , channelName ;

	//	create an animationClip for this updateCallback
	osgAnimation::Animation * animationClip = new osgAnimation::Animation() ;
	animationClip -> setName( clipName ) ;
	animationClip -> setWeight( 1.f ) ;
	_manager -> registerAnimation( animationClip ) ;

	for( unsigned int pa = 0 ; pa < _mKeyframeNode.length() ; ++pa ) 	{	// pa = plugArry
		MFnDagNode mfnDagNode( _mKeyframeNode[ pa ] , & _mStatus ) ;
		if ( _mStatus == MS::kSuccess )  {
			targetName = mfnDagNode.partialPathName( & _mStatus ).asChar() ;
			targetName.append( "_Callback" ) ;
		}

		osgAnimation::Channel * channel ;

		// Create corresponding template Channels
		// To prevent crash with animationClip -> addChannel : Create Sampler, Container, Dummy Keyframe
		if ( _mKeyframeAttr[ pa ] == 1  )  { // Rotate
			osgAnimation::QuatSphericalLinearChannel * channelQuat = new osgAnimation::QuatSphericalLinearChannel ;

			// Create Sampler, Container, Dummy Keyframe or animationClip -> addChannel crashes
			channelQuat -> getOrCreateSampler() -> getOrCreateKeyframeContainer()
				-> push_back( osgAnimation::QuatKeyframe( 0.f , osg::Quat( 0.f , 1.f , 0.f , 0.f ) ) ) ;

			channel = channelQuat ;

		}


		else if ( _mKeyframeAttr[ pa ] == 3  )  { // Color
			osgAnimation::Vec4LinearChannel * channelVec4 = new osgAnimation::Vec4LinearChannel ;

			// Create Sampler, Container, Dummy Keyframe or animationClip -> addChannel crashes
			channelVec4 -> getOrCreateSampler() -> getOrCreateKeyframeContainer()
				-> push_back( osgAnimation::Vec4Keyframe( 0.f , osg::Vec4( 0.f , 0.f , 0.f , 1.f) ) ) ;

			channel = channelVec4 ;

			// Shader is not a Dag Node, so treat as DG Node to get the name
			MFnDependencyNode mfnShaderNode( _mKeyframeNode[ pa ] , & _mStatus ) ;
			if ( _mStatus == MS::kSuccess )  {
				targetName = mfnShaderNode.name( & _mStatus ).asChar() ;
				targetName.append( "_Callback" ) ;
			}
		}

		else  { // Translate and Scale
			osgAnimation::Vec3LinearChannel * channelVec3 = new osgAnimation::Vec3LinearChannel ;

			// Create Sampler, Container, Dummy Keyframe or animationClip -> addChannel crashes
			channelVec3 -> getOrCreateSampler() -> getOrCreateKeyframeContainer()
				-> push_back( osgAnimation::Vec3Keyframe( 0.f , osg::Vec3( 0.f , 0.f , 0.f ) ) ) ;

			channel = channelVec3 ;
		}

		//	the name must be a certain keyword like "position" otherwise it does not work
		channel -> setName( _osgAttributes[ _mKeyframeAttr[ pa ] ] ) ;
		channel -> setTargetName( targetName ) ;
			
		animationClip -> addChannel( channel ) ;

	}


	sampleAttributeAnimation(	animationClip ) ;

}


///////////////////////////////////////////////////////////////////////////////////////////////////
// - Sample ---------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
void Animation::sampleAttributeAnimation(	osgAnimation::Animation * animationClip )
{
	// get the rotation for the bone this frame
	MTime				mTime ;
	MVector			mVector ;
	MQuaternion		mQuat , mOrient ;
	double3			mScale ;
	osg::Vec3		osgVec3 ;
	osg::Quat		osgQuat ;
	osg::Vec4		osgVec4 ;

	// Clear all Keys from previous usage, including first default keyframe
	for( unsigned int pa = 0 ; pa < _mKeyframeNode.length() ; ++pa ) 	{ // pa = plugArray
		osgAnimation::Vec3LinearChannel * channelVec3 = dynamic_cast< osgAnimation::Vec3LinearChannel * >( animationClip -> getChannels()[ pa ].get() ) ;
		if ( channelVec3 )  channelVec3 -> getOrCreateSampler() -> getOrCreateKeyframeContainer() -> clear() ;

		else  {
			osgAnimation::QuatSphericalLinearChannel * channelQuat = dynamic_cast< osgAnimation::QuatSphericalLinearChannel * >( animationClip -> getChannels()[ pa ].get() ) ;
			if ( channelQuat )  channelQuat -> getOrCreateSampler() -> getOrCreateKeyframeContainer() -> clear() ;
			
			else  {
				osgAnimation::Vec4LinearChannel * channelVec4 = dynamic_cast< osgAnimation::Vec4LinearChannel * >( animationClip -> getChannels()[ pa ].get() ) ;
				if ( channelVec4 )  channelVec4 -> getOrCreateSampler() -> getOrCreateKeyframeContainer() -> clear() ;
			}
		}
	}

	// Step through each frame of Animation Range
	for( int frame  = MAnimControl::minTime().as( MTime::uiUnit() ) ; 
				frame <= MAnimControl::maxTime().as( MTime::uiUnit() ) ; ++frame )
	{
		// set the current frame
		MAnimControl::setCurrentTime( MTime( frame , MTime::uiUnit() ) ) ;
		//_mStatus = mTime.setValue( ( double )frame ) ;
		mTime = MAnimControl::currentTime() - MAnimControl::minTime() ;


		// Loop through Plug Array to key frame each plug per frame 
		for( unsigned int pa = 0 ; pa < _mKeyframeNode.length() ; ++pa ) 	// pa = plugArray
		{
			MFnTransform	mfnTransform( _mKeyframeNode[ pa ] , & _mStatus ) ;
			MFnIkJoint		mfnJoint( _mKeyframeNode[ pa ] , & _mStatus  ) ;

			switch( _mKeyframeAttr[ pa ] )  {
				case 0 :							// --- Translate ---
					mVector = mfnTransform.getTranslation( MSpace::kTransform , & _mStatus ) ;
					osgVec3.set( osg::Vec3( mVector.x , mVector.y  , mVector.z ) ) ;
				break ;

				case 1 :							//	--- Rotate-Quat ---
					_mStatus = mfnTransform.getRotation( mQuat , _mQuatSpace ) ;

					// If transform is a Joint, jointOrient must be multiplied with rotation Quaternion			
					if ( _mKeyframeNode[ pa ].hasFn( MFn::kJoint ) )  {
						_mStatus = mfnJoint.getOrientation( mOrient ) ;
						mQuat = mQuat * mOrient ;
					}

					osgQuat.set( osg::Vec4f( mQuat.x , mQuat.y  , mQuat.z , mQuat.w ) ) ;
				break ;

				case 2 :							// --- Scale ---
					_mStatus = mfnTransform.getScale( mScale ) ;
					osgVec3.set( osg::Vec3( mScale[ 0 ] , mScale[ 1 ] , mScale[ 2 ] ) ) ;
				break ;

				case 3 :
					MString color , alpha ;

					if ( _mKeyframeNode[ pa ].hasFn( MFn::kSurfaceShader ) )  {
						color = "outColor" ;
						alpha = "outTransparency" ;
					}

					else  {
						color = "color" ;
						alpha = "transparency" ;
					}

					MFnDependencyNode mfnShaderNode( _mKeyframeNode[ pa ] , & _mStatus ) ;
					MPlug mPlugAlpha( mfnShaderNode.findPlug( alpha , true , & _mStatus ) ) ;
					mPlugAlpha.child( 0 ).getValue( osgVec4.r() ) ;
					mPlugAlpha.child( 1 ).getValue( osgVec4.g() ) ;
					mPlugAlpha.child( 2 ).getValue( osgVec4.b() ) ;

					osgVec4.a() = 1.0 - ( osgVec4.r() + osgVec4.g() + osgVec4.b() ) / 3.0f ;
					if ( osgVec4.a() > 0.999999f )  osgVec4.a() = 1.0f ;

					MPlug mPlugColor( mfnShaderNode.findPlug( color , true , & _mStatus ) ) ;
					mPlugColor.child( 0 ).getValue( osgVec4.r() ) ;
					mPlugColor.child( 1 ).getValue( osgVec4.g() ) ;
					mPlugColor.child( 2 ).getValue( osgVec4.b() ) ;

					break ;

				//default : break ;
			}


			// cast dynamically into a keyframeContainer of type osg::Vec3
			osgAnimation::Vec3LinearChannel * channelVec3 = 
				dynamic_cast< osgAnimation::Vec3LinearChannel * >( animationClip -> getChannels()[ pa ].get() ) ;

			// if success generate Keyframes for osg::Vec3
			if ( channelVec3 )  {
				doKeyframe< osg::Vec3 > 
					( channelVec3 -> getOrCreateSampler() -> getOrCreateKeyframeContainer() , osgVec3 , 3 , mTime.as( MTime::kSeconds ) ) ;
			}

			else  {	// cast dynamically into a keyframeContainer of type osg::Quat
				osgAnimation::QuatSphericalLinearChannel * channelQuat = 
					dynamic_cast< osgAnimation::QuatSphericalLinearChannel * >( animationClip -> getChannels()[ pa ].get() ) ;

				// if success generate Keyframes for osg::Quat
				if ( channelQuat )  {
				doKeyframe< osg::Quat >
					( channelQuat -> getOrCreateSampler() -> getOrCreateKeyframeContainer() , osgQuat , 4 , mTime.as( MTime::kSeconds ) ) ;
				}

				else  {	// cast dynamically into a keyframeContainer of type osg::Quat
					osgAnimation::Vec4LinearChannel * channelVec4 = 
						dynamic_cast< osgAnimation::Vec4LinearChannel * >( animationClip -> getChannels()[ pa ].get() ) ;
					// if success generate Keyframes for osg::Quat
					if ( channelVec4 )  {
						doKeyframe< osg::Vec4 > 
							( channelVec4 -> getOrCreateSampler() -> getOrCreateKeyframeContainer() , osgVec4 , 4 , mTime.as( MTime::kSeconds ) ) ;
					}
				}
			}
		}
	}

	// reset Maya time Slider to start time to get rid of nonzero attributes
	mTime = MAnimControl::minTime() ;
	MAnimControl::setCurrentTime( MAnimControl::minTime() ) ;
	//MAnimControl::setCurrentTime( MAnimControl::minTime().as( MTime::uiUnit() ) ) ;
	std::cout << std::endl << MAnimControl::minTime().as( MTime::uiUnit() ) << std::endl ;
	std::cout << std::endl << MAnimControl::minTime().as( MTime::kSeconds ) << std::endl ;

	// Duration of Animation Clip does not get written into osg file, so the next line does not work
	// so for now just don't remove first and last Keyframes of channels, especially the first one
	//animationClip -> setDuration( MAnimControl::maxTime().as( MTime::kSeconds ) - MAnimControl::minTime().as( MTime::kSeconds ) ) ;

}





///////////////////////////////////////////////////////////////////////////////////////////////////
// - Generate -------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
bool Animation::exporta()
{
	//	add animationClip to updateCallback
	//	Maya Scene has Animation
	if ( !_mKeyframeNode.length() ) 
		return false ;


	MObject		mClip ;
	MObject		mCharacter ;
	MDGModifier	mdgModifier ;
	std::vector< bool > clipConfig ;
	MItDependencyNodes mitDG( MFn::kCharacter ) ;

	// If Maya Scene has no Character, sample the scene one time
	if ( mitDG.isDone() )  {
		addAnimationClip( "No Character" ) ;	// Add osgAnimation and sample Maya Animation
		return true ;
	}
	
	for( ; !mitDG.isDone() ; mitDG.next() )  {
		MFnCharacter mfnCharacter( mitDG.item() , & _mStatus ) ;
		unsigned int clipCount = mfnCharacter.getScheduledClipCount( & _mStatus ) ;

		// If Character has no clips, sample the scene one time
		if ( !clipCount )  {
			addAnimationClip( mfnCharacter.name( & _mStatus ).asChar() ) ;	// Add osgAnimation and sample Maya Animation
			return true ;
		}

		clipConfig.resize( clipCount ) ;

		// Store Clip Configuration ( on / off ) of each clip and turn each of
		for( unsigned int cc = 0 ; cc < clipCount ; ++cc )  { // cc = clipCounter
			MFnClip mfnClip( mfnCharacter.getScheduledClip( cc ) , & _mStatus ) ;
			clipConfig[ cc ] = mfnClip.getEnabled() ;
			mfnClip.setEnabled( false , & mdgModifier ) ;
		}

		// Loop through all clips, enable, capture animationClip, disable
		for( unsigned int cc = 0 ; cc < clipCount ; ++cc )  { // cc = clipCounter
			MFnClip mfnClip( mfnCharacter.getScheduledClip( cc ) , & _mStatus ) ;

			mfnClip.setEnabled( true  , & mdgModifier ) ;				//	Enable single Clip
			addAnimationClip( mfnClip.name( & _mStatus ).asChar() ) ;	// Add osgAnimation and sample Maya Animation
			mfnClip.setEnabled( false , & mdgModifier ) ;				//	Disable the Clip
		}

		// Restore original Clip Configuration ( on / off ) of each clip
		for( unsigned int cc = 0 ; cc < clipCount ; ++cc )  { // cc = clipCounter
			MFnClip mfnClip( mfnCharacter.getScheduledClip( cc ) , & _mStatus ) ;
			mfnClip.setEnabled( clipConfig[ cc ] , & mdgModifier ) ;
		}
	}

	return true ;

}

