#ifndef _ANIMATION_H_
#define _ANIMATION_H_


#include "utility.h"


// ----- Maya -----------------------------------------------------------------------
#include <maya/MTime.h>
//#include <maya/MDagNode.h>
//#include <maya/MDagPath.h>
#include <maya/MIntArray.h>
#include <maya/MPlugArray.h>
#include <maya/MAnimControl.h>
#include <maya/MDoubleArray.h>
#include <maya/MObjectArray.h>
#include <maya/MStringArray.h>


// ----- OSG ------------------------------------------------------------------------
//#include <osg/Group>
//#include <osg/Object>
#include <osg/Material>
#include <osg/MatrixTransform>

#include <osgAnimation/Keyframe>
#include <osgAnimation/Channel>
#include <osgAnimation/BasicAnimationManager>


//#include <sys/types.h>
//#include <string>
//#include <vector>

#include <fstream>
#include <sstream>


class Animation
{
public :
	//Animation( std::ofstream &	logFile ) ;
	//Animation( std::ofstream &	logFile , osg::ref_ptr< osgAnimation::BasicAnimationManager > animationManager ) ;

	static void	mapInputConnections(	const MObject & mObject , osg::ref_ptr< osg::MatrixTransform > & osgXForm ) ;
	static void	mapInputConnections( const MObject & mObject , osg::ref_ptr< osg::Material > & osgMaterial ) ;
	static bool	exporta() ;	// formally generateKeyframesy

	static osg::ref_ptr< osgAnimation::BasicAnimationManager > getManager() { return _manager ; }

	static void init() ;

private :
	static void	addAnimationClip( const std::string clipName ) ;
	static void	sampleAttributeAnimation( osgAnimation::Animation * animation ) ;

	static bool				_isInited ;
	static double			_slopeTollerance ;	//	key if slope between keys changes more then this value
	static MStatus			_mStatus ;
	static MObjectArray	_mKeyframeNode ;		// holds all DAG Paths for key framing
	static MIntArray		_mKeyframeAttr ;		// holds the corresponding Pointer into keyAttributes ( Types, e.g. "translate" ) 
	static MStringArray	_keyAttributes ;		// Array of Attributes for keying
	static MDoubleArray	_keyDefaults ;			// Holds default Values for keyAttributes
	static MSpace::Space	_mQuatSpace ;

	static std::vector< std::string >	_osgAttributes ;	// Holds defining osg channel names
	static osg::ref_ptr< osgAnimation::BasicAnimationManager > _manager ;



	//inline bool inTollerance( double base , double test ) const ;

	// ToDo :
	// - First Channel in Animation must have length of entire animation, otherwise playback is corrupted

	// VEC	: Type of Vector, either osg::Quat or osg::Vec3
	template < typename VEC >
	static void doKeyframe(  osgAnimation::TemplateKeyframeContainer< VEC > * keys , 
		VEC						osgVec ,
		const unsigned int	numComps ,
		const	double			time )
	{
		bool	sameSlope ;
		osgAnimation::TemplateKeyframe< VEC > * key1 , * key2 ;

		if ( keys -> size() > 1 )  {	// at least 2 key frames need to be present already to check the slope
			sameSlope = true ;		// assume that the slope is the same for all components of two key frames
			key1 = & keys -> at( keys -> size() - 1 ) ;	// handle to last key frame
			key2 = & keys -> at( keys -> size() - 2 ) ;	// handle to last but one key frame

			for( unsigned int vs = 0 ; vs < numComps ; ++vs )  {	// loop through each Component of the Vec3
				sameSlope = sameSlope && inTollerance(			// check if slope of last key frames is in tolerance with slope of current and last
					( key1 -> getValue()[ vs ]	- key2 -> getValue()[ vs ] ) / ( key1 -> getTime()	- key2 -> getTime() ) , 
					( osgVec[ vs ]					- key1 -> getValue()[ vs ] ) / ( time					- key1 -> getTime() ) ,
					_slopeTollerance ) ;
			}

			// if the slope is the same, delete the last key frame
			if ( sameSlope )
				keys -> pop_back() ;
		}

		// now key at the current time with the current values
		keys -> push_back( osgAnimation::TemplateKeyframe< VEC >( time , osgVec ) ) ;

		// Unfortunately Animation plays wrong, if the first channel has not length of entire Animation
		// Use this Feature in a later version, where playback might work right
		// Also remove the first key, if not necessary
		// get rid of last key frame if slope is horizontal
/*		if ( frame == MAnimControl::maxTime().as( MTime::uiUnit() ) && keys -> size() > 1 )  {
			sameSlope = true ;		// use Variable for checking each component though
			key1 = & keys -> at( keys -> size() - 1 ) ;	// handle to last key frame
			key2 = & keys -> at( keys -> size() - 2 ) ;	// handle to last but one key frame

			for( unsigned int vs = 0 ; vs < 3 ; ++vs )  {	// loop through each Component of the Vec3, to check if values are inTolerance
				sameSlope = sameSlope && inTollerance( key1 -> getValue()[ vs ] , key2 -> getValue()[ vs ] , _slopeTollerance ) ;
			}

			// if last and last but one keyframes are the same, delete the last one
			if( sameSlope )
				keys -> pop_back() ;
		}*/
	}
} ;


#endif	// _ANIMATION_H_