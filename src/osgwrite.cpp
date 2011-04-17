#include "osgwrite.h"
#include "config.h"

#include "pointemitter.h"
#include "common.h"
#include "maya2osg.h"
#include "dagnode.h"
#include "texture.h"
#include "lights.h"
#include "camera.h"
#include "particle.h"
#include "shadows.h"
#include "animation.h"

#include <maya/MGlobal.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyNodes.h>

#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <osgDB/Output>
#include <osgDB/WriteFile>
#include <osgDB/FileNameUtils>

#include <stdlib.h>
#include <string.h>

//#include <iostream>
#include <fstream>
#include <sstream>

// Log Steps and Stages
osgDB::Output logFile ;


// Work in progress: to convert textures to hardware compressed format, we need a GL context
//#define CREATE_GL_CONTEXT
#ifdef CREATE_GL_CONTEXT
#  include <osgViewer/Viewer>
#  include <osg/ArgumentParser>
#endif


/**
 *	Export
 */
MString OSGWrite::parseArgs( const MStringArray & argStringArray )
{
	//MStatus status;
	MString filename ;

	filename = "model.osg";
	for( unsigned int i = 0 ; i < argStringArray.length() ; ++i )  {

		if ( argStringArray[ i ] == "-exportSelection" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setExportSelection( false );
			}
			else {
				Config::instance()->setExportSelection( true );
			}
		}
		else if( argStringArray[ i ] == "-texenv" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "DECAL" ){
				Config::instance()->setMode(osg::TexEnv::DECAL);
			}
			else if( argStringArray[ i ] == "MODULATE" ){
				Config::instance()->setMode(osg::TexEnv::MODULATE);
			}
			else if( argStringArray[ i ] == "BLEND" ){
				Config::instance()->setMode(osg::TexEnv::BLEND);
			}
			else if( argStringArray[ i ] == "REPLACE" ){
				Config::instance()->setMode(osg::TexEnv::REPLACE);
			}
			else if( argStringArray[ i ] == "ADD" ){
				Config::instance()->setMode(osg::TexEnv::ADD);
			}
			else{
				std::cerr << "ERROR. No valid \"TexEnv mode\" defined. Assuming REPLACE mode." << std::endl;
				Config::instance()->setMode(osg::TexEnv::REPLACE);
			}
		}
		else if ( argStringArray[ i ] == "-exportDefaultCameras" ) {
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setExportDefaultCameras( false );
			}
			else {
				Config::instance()->setExportDefaultCameras( true );
			}
		}
		else if ( argStringArray[ i ] == "-exportOrthoCameras" ) {
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setExportOrthographicCameras( false );
			}
			else {
				Config::instance()->setExportOrthographicCameras( true );
			}
		}
		else if ( argStringArray[ i ] == "-surfaceMode" ) {
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "SINGLE" ){
				Config::instance()->setSurfaceMode(Config::SINGLE);
			}
			else if( argStringArray[ i ] == "DOUBLE" ){
				Config::instance()->setSurfaceMode(Config::DOUBLE);
			}
			else if( argStringArray[ i ] == "KEEP" ){
				Config::instance()->setSurfaceMode(Config::KEEP);
			}
		}
		else if ( argStringArray[ i ] == "-texClampMode" ) {
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "EDGE" ){
				Config::instance()->setTexClampMode(Config::EDGE);
			}
			else if( argStringArray[ i ] == "COLOR" ){
				Config::instance()->setTexClampMode(Config::COLOR);
			}
		}
		else if ( argStringArray[ i ] == "-YUp2ZUp" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setYUp2ZUp( false );
			}
			else {
				Config::instance()->setYUp2ZUp( true );
			}
		}
		else if ( argStringArray[ i ] == "-exportAnim" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setExportAnimations( false );
			}
			else {
				Config::instance()->setExportAnimations( true );
			}
		}
		else if ( argStringArray[ i ] == "-animTransformType" ) {
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "ANIMATION_PATH" ){
				Config::instance()->setAnimTransformType(Config::ANIMATION_PATH);
			}
			else if( argStringArray[ i ] == "OSG_ANIMATION" ){
				Config::instance()->setAnimTransformType(Config::OSG_ANIMATION);
			}
		}
		else if ( argStringArray[ i ] == "-animSampleBy" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			Config::instance()->setAnimSampleBy( argStringArray[ i ].asInt() ) ;
		}
		else if ( argStringArray[ i ] == "-particlesEmissive" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setParticlesEmissive( false );
			}
			else {
				Config::instance()->setParticlesEmissive( true );
			}
		}
		else if ( argStringArray[ i ] == "-particlesLighting" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setParticlesLighting( false );
			}
			else {
				Config::instance()->setParticlesLighting( true );
			}
		}
		else if ( argStringArray[ i ] == "-computeShadows" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setComputeShadows( false );
			}
			else {
				Config::instance()->setComputeShadows( true );
			}
		}
		else if ( argStringArray[ i ] == "-shadowTechnique" ) {
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "SHADOW_MAP" ){
				Config::instance()->setShadowTechnique(Config::SHADOW_MAP);
			}
			else if( argStringArray[ i ] == "SOFT_SHADOW_MAP" ){
				Config::instance()->setShadowTechnique(Config::SOFT_SHADOW_MAP);
			}
			else if( argStringArray[ i ] == "SHADOW_VOLUME" ){
				Config::instance()->setShadowTechnique(Config::SHADOW_VOLUME);
			}
			else if( argStringArray[ i ] == "SHADOW_TEXTURE" ){
				Config::instance()->setShadowTechnique(Config::SHADOW_TEXTURE);
			}
			else if( argStringArray[ i ] == "PARALLEL_SPLIT_SHADOW_MAP" ){
				Config::instance()->setShadowTechnique(Config::PARALLEL_SPLIT_SHADOW_MAP);
			}
		}
		else if ( argStringArray[ i ] == "-globalAmbient" ){
			if(i+3 >= argStringArray.length())
				break;
			float x = argStringArray[ ++i ].asDouble() ;
			float y = argStringArray[ ++i ].asDouble() ;
			float z = argStringArray[ ++i ].asDouble() ;
			osg::Vec3 ambient_color( x, y, z );
			Config::instance()->setGlobalAmbient( ambient_color );
		}
		else if ( argStringArray[ i ] == "-localViewer" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setLocalViewer( false );
			}
			else {
				Config::instance()->setLocalViewer( true );
			}
		}
		else if ( argStringArray[ i ] == "-useGLSL" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setUseGLSL( false );
			}
			else {
				Config::instance()->setUseGLSL( true );
			}
		}
		else if ( argStringArray[ i ] == "-exportNormals" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setExportNormals( false );
			}
			else {
				Config::instance()->setExportNormals( true );
			}
		}
		else if ( argStringArray[ i ] == "-exportTexCoords" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setExportTexCoords( false );
			}
			else {
				Config::instance()->setExportTexCoords( true );
			}
		}
		else if ( argStringArray[ i ] == "-maxAnisotropy" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			Config::instance()->setMaxAnisotropy(argStringArray[ i ].asInt() ) ;
		}
		else if ( argStringArray[ i ] == "-enableBumpMapping" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setEnableBumpMapping( false );
			}
			else {
				Config::instance()->setEnableBumpMapping( true );
			}
		}
		else if ( argStringArray[ i ] == "-runTimeBumpDepth" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setRunTimeBumpDepth( false );
			}
			else {
				Config::instance()->setRunTimeBumpDepth( true );
			}
		}
		else if ( argStringArray[ i ] == "-regenerateTextures" ){
			i++;
			if ( i == argStringArray.length() )
				break;
			if( argStringArray[ i ] == "0" ){
				Config::instance()->setRegenerateTextures( false );
			}
			else {
				Config::instance()->setRegenerateTextures( true );
			}
		}
		else if ( argStringArray[ i ].length() > 0 ) {
			// We discard empty arguments
			filename = argStringArray[ i ];
		}
	}

	return filename ;
}



MStatus OSGWrite::exporta( const MString & filename )
{

#ifdef CREATE_GL_CONTEXT
/*    int argc = 1;
    char *argv[1] = { "arg0" };
    osg::ArgumentParser viewer_args(&argc, argv);
    osgViewer::Viewer viewer( viewer_args );
    viewer.setThreadingModel( osgViewer::ViewerBase::SingleThreaded );*/
	osg::GraphicsContext::Traits *traits = new osg::GraphicsContext::Traits();
    traits->x = traits->y = 0;
    traits->width = traits->height = 1;
    traits->windowDecoration = false;
	osg::GraphicsContext *gc = osg::GraphicsContext::createGraphicsContext( traits );
//    viewer.getCamera()->setGraphicsContext(gc);
//    viewer.realize();
    gc->realize();
    gc->makeCurrent();
#endif


	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---      Maya2OSG - OSG Maya Exporter           ---" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "--  OUTPUT FILE : " << filename.asChar() << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;

	MStatus status;
	MSelectionList sel;

	status = MGlobal::getActiveSelectionList( sel );
	if ( MCheckStatus(status, "ERROR querying selection") ) {
		return MS::kFailure;
	}
	std::cout << sel.length() << " element(s) selected" << std::endl;

    // Create the OSG scene graph and dump to disk

	osg::ref_ptr<osg::Group> root;

	if ( Config::instance()->getYUp2ZUp() ) {
		osg::PositionAttitudeTransform *pat = new osg::PositionAttitudeTransform();
		osg::Quat q;
		q.makeRotate( osg::Vec3(0.0, 1.0, 0.0), osg::Vec3(0.0, 0.0, 1.0) );
		pat->setAttitude( q );
		root = pat;
	}
	else {
		root = new osg::Group();
	}

	// Set the base name to the Config (used to export Camera and CameraAnimation files)
	Config::instance()->setSceneFilePath( filename.asChar() );

	// Scene root node name -> file base name
	root->setName( Config::instance()->getSceneFilePath().getFileBaseName() );

	// Open logFile where Scene gets exported
	logFile.open( ( Config::instance()->getSceneFilePath().getDirectory() + "/Maya2OSG_Write.log" ).c_str() ) ;
	
	// Create New AnimationManager
	// Initialize Attribute Arrays required once on Plug-In load
	Animation::init() ;

	// Malfunctioning, Objects get exported multiple times, if
	// several objects are selected which are children of each other
	if ( Config::instance()->getExportSelection() && sel.length() > 0 ) {
		// If there is a selection, export only the selected elements
		for( int i=0 ; i < sel.length() ; i++ ) {
			MDagPath dagPath;
			sel.getDagPath(i, dagPath);
//			std::cout << "SELECTION - ELEMENT " << i << " PATH = " << dagPath.fullPathName().asChar() << std::endl;
			osg::ref_ptr<osg::Node> scene = DAGNode::exporta(dagPath);
            if ( scene.valid() )
    			root->addChild(scene.get());
		}

		// all collected Plugs will be captured in keyframes once per Maya Character Clip
		if ( Config::instance() -> getAnimTransformType() == Config::OSG_ANIMATION )  {
			if ( Animation::exporta() )  {
				Animation::getManager() -> buildTargetReference() ;			
				root -> addUpdateCallback( Animation::getManager().get() ) ;	//	add AnimationManager from Animation Object to root node
			}
		}
	}

	else {
		// If there is no selection, export all the scene

		// Trick to get a path to the root of the DAG
		MItDag dagIt;
		MDagPath dagPath;
		status = MDagPath::getAPathTo(dagIt.root(), dagPath);
		if ( MCheckStatus(status, "ERROR requesting path to DAG root node") ) {
			return MS::kFailure;
		}

		// Create New AnimationManager
		// Initialize Attribute Arrays required once on Plug-In load
		Animation::init() ;

		// all animated Transform Plugs will be collected
		osg::ref_ptr<osg::Node> scene = DAGNode::exporta(dagPath);
		if ( scene.valid() )
    		root->addChild(scene.get());

		// all collected Plugs will be captured in keyframes once per Maya Character Clip
		if ( Config::instance() -> getAnimTransformType() == Config::OSG_ANIMATION )  {
			if ( Animation::exporta() )  {
				Animation::getManager() -> buildTargetReference() ;			
				root -> addUpdateCallback( Animation::getManager().get() ) ;	//	add AnimationManager from Animation Object to root node
			}
		}
	}

	osg::ref_ptr<osg::StateSet> st = root->getOrCreateStateSet();
	// TRUCO DEL ALMENDRUCO: Hay que decirle que normalice las normales
    st->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
	// Create the states so lights affect the whole scene
	Lights::configureStateSet(st);
	// Configure the uniforms related to lighting
	Lights::configureUniforms();

	// Cast shadows
	if ( Config::instance()->getComputeShadows() ) {
		osg::ref_ptr<osg::Group> shadow_group = Shadows::createShadowGroup();
		if ( shadow_group.valid() ) {
			shadow_group->addChild( root );
			root = shadow_group;
		}
	}

	// If there are particle systems, attach the ParticleSystemUpdater
	// at the end of the scene graph
	root->addChild( Particle::getParticleSystemUpdater() );
	// and bind created emitters to particle systems
	PointEmitter::bindEmittersToParticles();

	// Write scene to disk file
	osgDB::writeNodeFile(*root.get(), filename.asChar());

	/////////////////////////////////////////////////////////////
	// Reset scene-related configuration to default state

	// Empty the texture cache to allow texture modifications in later exportations
	Texture::deleteCaches();
	// Delete accumulated lights in the scene so list is empty for later exportations
	Lights::reset();

	// Clear particle systems related information
	Particle::reset();
	PointEmitter::reset();

	// Clear register of lights casting shadows
	Shadows::reset();

	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---      Exportation successfully completed     ---" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;

	logFile.close() ;

	return MStatus::kSuccess;


}
