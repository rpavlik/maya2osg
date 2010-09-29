/**
    Maya2OSG - A toolkit for exporting Maya scenes to OpenSceneGraph
    Copyright (C) 2010 Javier Taibo <javier.taibo@gmail.com>

    This file is part of Maya2OSG.

    Maya2OSG is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Maya2OSG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Maya2OSG.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "pointemitter.h"
#include "common.h"
#include "maya2osg.h"
#include "config.h"
#include "dagnode.h"
#include "texture.h"
#include "lights.h"
#include "camera.h"
#include "particle.h"

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
#include <osgDB/WriteFile>

#include <stdlib.h>
#include <string.h>

#include <iostream>

void* maya2osg::creator()
{
	return new maya2osg();
}

MStatus maya2osg::doIt( const MArgList & args )
{
	MStatus status;
	MString filename;

	filename = "model.osg";
	for(int i=0; i<args.length(); i++){

		if( args.asString(i) == "-texenv" ){
			i++;
			if(i==args.length())
				break;
			if( args.asString(i) == "DECAL" ){
				Config::instance()->setMode(osg::TexEnv::DECAL);
			}
			else if( args.asString(i) == "MODULATE" ){
				Config::instance()->setMode(osg::TexEnv::MODULATE);
			}
			else if( args.asString(i) == "BLEND" ){
				Config::instance()->setMode(osg::TexEnv::BLEND);
			}
			else if( args.asString(i) == "REPLACE" ){
				Config::instance()->setMode(osg::TexEnv::REPLACE);
			}
			else if( args.asString(i) == "ADD" ){
				Config::instance()->setMode(osg::TexEnv::ADD);
			}
			else{
				std::cerr << "ERROR. No valid \"TexEnv mode\" defined. Assuming REPLACE mode." << std::endl;
				Config::instance()->setMode(osg::TexEnv::REPLACE);
			}
		}
		else if ( args.asString(i) == "-exportDefaultCameras" ) {
			i++;
			if(i==args.length())
				break;
			if( args.asString(i) == "0" ){
				Config::instance()->setExportDefaultCameras( false );
			}
			else {
				Config::instance()->setExportDefaultCameras( true );
			}
		}
		else if ( args.asString(i) == "-exportOrthoCameras" ) {
			i++;
			if(i==args.length())
				break;
			if( args.asString(i) == "0" ){
				Config::instance()->setExportOrthographicCameras( false );
			}
			else {
				Config::instance()->setExportOrthographicCameras( true );
			}
		}
		else if ( args.asString(i) == "-surfaceMode" ) {
			i++;
			if(i==args.length())
				break;
			if( args.asString(i) == "SINGLE" ){
				Config::instance()->setSurfaceMode(Config::SINGLE);
			}
			else if( args.asString(i) == "DOUBLE" ){
				Config::instance()->setSurfaceMode(Config::DOUBLE);
			}
			else if( args.asString(i) == "KEEP" ){
				Config::instance()->setSurfaceMode(Config::KEEP);
			}
		}
		else if ( args.asString(i) == "-texClampMode" ) {
			i++;
			if(i==args.length())
				break;
			if( args.asString(i) == "EDGE" ){
				Config::instance()->setTexClampMode(Config::EDGE);
			}
			else if( args.asString(i) == "COLOR" ){
				Config::instance()->setTexClampMode(Config::COLOR);
			}
		}
		else if ( args.asString(i) == "-YUp2ZUp" ){
			i++;
			if(i==args.length())
				break;
			if( args.asString(i) == "0" ){
				Config::instance()->setYUp2ZUp( false );
			}
			else {
				Config::instance()->setYUp2ZUp( true );
			}
		}
		else if ( args.asString(i) == "-exportAnim" ){
			i++;
			if(i==args.length())
				break;
			if( args.asString(i) == "0" ){
				Config::instance()->setExportAnimations( false );
			}
			else {
				Config::instance()->setExportAnimations( true );
			}
		}
		else if ( args.asString(i) == "-animSampleBy" ){
			i++;
			if(i==args.length())
				break;
			Config::instance()->setAnimSampleBy( args.asInt(i) );
		}
		else if ( args.asString(i) == "-particlesEmissive" ){
			i++;
			if(i==args.length())
				break;
			if( args.asString(i) == "0" ){
				Config::instance()->setParticlesEmissive( false );
			}
			else {
				Config::instance()->setParticlesEmissive( true );
			}
		}
		else if ( args.asString(i) == "-particlesLighting" ){
			i++;
			if(i==args.length())
				break;
			if( args.asString(i) == "0" ){
				Config::instance()->setParticlesLighting( false );
			}
			else {
				Config::instance()->setParticlesLighting( true );
			}
		}
		else if ( args.asString(i).length() > 0 ) {
			// We discard empty arguments
			filename = args.asString(i);
		}
	}

	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---      Maya2OSG - OSG Maya Exporter           ---" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "--  OUTPUT FILE : " << filename.asChar() << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;

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

	// Remove file extension (if any)
#ifdef WIN32
	char *filebasename = (char *)malloc(strlen(filename.asChar()));
	_splitpath(filename.asChar(), NULL, NULL, filebasename, NULL);
#else
	char *filebasename = strdup(filename.asChar());
	filebasename = basename(filebasename);
	if(strrchr(filebasename, '.')!=NULL)
		*(strrchr(filebasename, '.'))=0;
#endif
	// This operation could have been done with any of the
	// osgDB functions, such as osgDB::getSimpleFileName

	root->setName(filebasename);
	// Set the base name to the Camera exporter
	Camera::setSceneFileBaseName( filebasename );

	if ( sel.length() > 0 ) {
		// If there is a selection, export only the selected elements
		for( int i=0 ; i < sel.length() ; i++ ) {
			MDagPath dagPath;
			sel.getDagPath(i, dagPath);
//			std::cout << "SELECTION - ELEMENT " << i << " PATH = " << dagPath.fullPathName().asChar() << std::endl;
			osg::ref_ptr<osg::Node> scene = DAGNode::exporta(dagPath);
			root->addChild(scene.get());
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

		osg::ref_ptr<osg::Node> scene = DAGNode::exporta(dagPath);
		root->addChild(scene.get());
	}

	osg::ref_ptr<osg::StateSet> st = root->getOrCreateStateSet();
	// TRUCO DEL ALMENDRUCO: Hay que decirle que normalice las normales
    st->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
	// Create the states so lights affect the whole scene
	Lights::configureStateSet(st);

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

	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---      Exportation succesfully completed      ---" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	return MStatus::kSuccess;
}
