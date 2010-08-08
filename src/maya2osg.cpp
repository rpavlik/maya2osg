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
#include <maya/MSimple.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyNodes.h>

#include <osg/Node>
#include <osg/Group>
#include <osgDB/WriteFile>
#include <osg/StateSet>
#include <osg/StateAttribute>

#include "config.h"
#include "dagnode.h"
#include "texture.h"
#include "lights.h"

#include <stdlib.h>
#include <string.h>

#include <iostream>

#define VERSION "0.2.0"

#define MCheckStatus(st, msg) \
	if( (st).statusCode() != MS::kSuccess )\
	{\
	std::cout << msg << std::endl;\
	return MS::kFailure;\
	}

DeclareSimpleCommand( maya2osg, "Maya2OSG", VERSION );


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
		else {
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
	MCheckStatus(status, "ERROR querying selection");
	std::cout << sel.length() << " element(s) selected" << std::endl;

    // Create the OSG scene graph and dump to disk
	osg::ref_ptr<osg::Group> root = new osg::Group();

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
	root->setName(filebasename);

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
		MCheckStatus(status, "ERROR requesting path to DAG root node");

		osg::ref_ptr<osg::Node> scene = DAGNode::exporta(dagPath);
		root->addChild(scene.get());
	}

	osg::ref_ptr<osg::StateSet> st = root->getOrCreateStateSet();
	// TRUCO DEL ALMENDRUCO: Hay que decirle que normalice las normales
    st->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
	// Create the states so lights affect the whole scene
	Lights::configureStateSet(st);

	// Write scene to disk file
	osgDB::writeNodeFile(*root.get(), filename.asChar());

	// Empty the texture cache to allow texture modifications in later exportations
	Texture::deleteCaches();
	// Delete accumulated lights in the scene so list is empty for later exportations
	Lights::reset();

	std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "---      Exportation succesfully completed      ---" << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
	return MStatus::kSuccess;
}
