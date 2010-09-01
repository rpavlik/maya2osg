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

#include "common.h"
#include "osg2maya.h"
#include "importervisitor.h"
#include "proxymaterials.h"

#include <maya/MArgList.h>

#include <osgDB/ReadFile>
#include <osg/Node>

#include <iostream>


void* osg2maya::creator()
{
	return new osg2maya;
}

MStatus osg2maya::doIt( const MArgList & args )
{
	MString filename;

	// Argument: name of the OSG file to import

	if ( args.length() < 1 ){
		std::cerr << "No file name supplied" << std::endl;
		return MStatus::kFailure;
	}
	else {
		filename = args.asString(0);
	}

	// Import file...
	std::cout << "===============================================================================================" << std::endl;
	std::cout << "Importing OSG file : " << filename.asChar() << std::endl;
	std::cout << "===============================================================================================" << std::endl;

	osg::ref_ptr<osg::Node> scene = osgDB::readNodeFile(filename.asChar());
	if ( ! scene.valid() ){
		std::cerr << "ERROR. Reading file " << filename.asChar() << std::endl;
		return MStatus::kFailure;
	}

	ImporterVisitor visitor;
	scene->accept(visitor);

	// Empty caches and stuff...
	ProxyMaterials::clear();

	std::cout << "===============================================================================================" << std::endl;
	std::cout << "File " << filename.asChar() << " succesfully imported" << std::endl;
	std::cout << "===============================================================================================" << std::endl;

	return MStatus::kSuccess;
}
