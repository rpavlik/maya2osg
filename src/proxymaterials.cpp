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
#include "proxymaterials.h"

#include <maya/MFnLambertShader.h>
#include <maya/MFnSet.h>
#include <maya/MSelectionList.h>
#include <maya/MDGModifier.h>
#include <maya/MPlug.h>

#include <iostream>

// Map of materials
std::map<void *, MObject> ProxyMaterials::_shaderMap;

#define ERROR_CHECK(status,msg)	if(status!=MStatus::kSuccess) { std::cout << "ERROR : " << msg << std::endl; }

/**
 *	Get the Maya shader bound to an OSG StateSet
 */
MObject &ProxyMaterials::getShader(osg::StateSet *ss)
{
	// If it exists, the MObject is returned
	if( _shaderMap.find(ss) != _shaderMap.end() ){
#ifdef _DEBUG
		std::cout << "Material is cached!" << std::endl;
#endif
		return _shaderMap[ss];
	}

	// Create the shader

	std::cout << "New material. Constructing..." << std::endl;
	MSelectionList selection_list;
	MFnSet mfn_set;
	MStatus status;
	MObject shader = mfn_set.create(selection_list, MFnSet::kRenderableOnly, status);
	ERROR_CHECK(status,"MFnSet::create()")

	MFnLambertShader mfn_lambert;
	mfn_lambert.create();
//	mfn_lambert.

	// Connect the Material to the ShadingEngine
	
	MFnDependencyNode dnfn_material(mfn_lambert.object());
	MFnDependencyNode dnfn_shader(shader);
	MPlug plug_material = dnfn_material.findPlug("outColor",&status);
	ERROR_CHECK(status,"Seeking material 'plug'")
	MPlug plug_shader = dnfn_shader.findPlug("surfaceShader",&status);
	ERROR_CHECK(status,"Seeking shader 'plug'")
	MDGModifier dgmod;
	status = dgmod.connect(plug_material, plug_shader);
	ERROR_CHECK(status,"MDGModifier::connect()")

	_shaderMap[ss] = shader;
	return _shaderMap[ss];
}
