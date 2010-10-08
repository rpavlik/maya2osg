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
#ifndef _SHADER_H_
#define _SHADER_H_

#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <osg/StateSet>
#include <osg/Material>

class Shader {

public:
	/// Create and return the StateSet of the ShadingEngine/ShadingGroup object
	static osg::ref_ptr<osg::StateSet> exporta(const MObject &shading_engine, const MObjectArray &textures);

	/// Get the shading engine applied to the dependency node 
	static void getShadingEngine(const MObject &dependency_node, MObject &shading_engine);

	/// Get the surface shader applied to the shading engine
	static void getSurfaceShader(const MObject &shading_engine, MObject &surface_shader);

	/// Gather the textures applied to a surface shader
	static void getTextures( const MObject &surface_shader, MObjectArray &textures );

	/// Get the file texture connected to color channel (if any)
	static void getColorTexture( const MObject &surface_shader, MObject &texture );

	/// Check if the material has any texture connected to the specified channel
	static bool connectedTexture(const MObject &surface_shader, std::string channel);

private:
	/// Create OSG/OpenGL material corresponding to Maya material
	static osg::ref_ptr<osg::Material> material(const MObject &obj, bool &mat_trans);
	/// Add textures to the StateSet
	static void setTextures(osg::ref_ptr<osg::StateSet> st, const MObjectArray &textures, bool tex_trans);

};

#endif //_SHADER_H_
