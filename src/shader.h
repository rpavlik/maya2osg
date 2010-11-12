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
#include <maya/MPlug.h>
#include <osg/StateSet>
#include <osg/Material>

class Shader {

public:
	/// Create and return the StateSet of the ShadingEngine/ShadingGroup object
	static void exporta(const MObject &shading_engine, osg::StateSet &state_set);

	/// Get the shading engine applied to the dependency node 
	static void getShadingEngine(const MObject &dependency_node, MObject &shading_engine);

	/// Get the surface shader applied to the shading engine
	static void getSurfaceShader(const MObject &shading_engine, MObject &surface_shader);

	/// Get the file texture connected to color channel (if any)
	static void getColorTexture( const MObject &surface_shader, MObject &texture );

	/// Check if a Shading Node has any node connected to the specified channel
	static bool connectedChannel(const MObject &shading_node, std::string channel);

    /// Get the node connected to a channel of a shading node
    static void getNodeConnectedToChannel( const MObject &shading_node, std::string channel, MObject &node );

    /// Get the plug connected to a channel of a shading node
    static void getPlugConnectedToChannel( const MObject &shading_node, std::string channel, MPlug &remote_plug );

    /// Create OSG/OpenGL material corresponding to Maya material
	static osg::ref_ptr<osg::Material> material(const MObject &surface_shader, bool &mat_trans);

private:
    /**
     *  Establish the color (and maybe transparency) texture
     *  Texture is bound to texture unit 0 unless otherwise explicited in the "texture_unit" parameter
     *  @return Color texture contains transparency/opacity/alpha channel
     */
    static bool setColorTexture(osg::StateSet &st, const MObject &surface_shader, int texture_unit=0);

};

#endif //_SHADER_H_
