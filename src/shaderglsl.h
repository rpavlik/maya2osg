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
#ifndef _SHADERGLSL_H_
#define _SHADERGLSL_H_

#include <maya/MObject.h>
#include <maya/MObjectArray.h>

#include <osg/StateSet>


/**
 *	ShaderGLSL class works similar to Shader class, but instead of OpenGL materials
 *	it uses GLSL shaders to emulate the visual quality of "Maya soft" renderer.
 */
class ShaderGLSL {

public:
	/// Configure the shaders for the StateSet of the ShadingEngine/ShadingGroup object
	static void exporta(const MObject &shading_engine, const MObjectArray &textures,
						osg::ref_ptr<osg::StateSet> state_set);

private:
	/// Create the stateset for a Lambert material
	static void exportLambert(const MObject &surface_shader, osg::ref_ptr<osg::StateSet> state_set);
};

#endif //_SHADERGLSL_H_
