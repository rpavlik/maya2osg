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
#ifndef _LAMBERT_H_
#define _LAMBERT_H_

#include <maya/MObject.h>

#include "glslcodeblock.h"
#include "../shaderglsl.h"

#include <string>

class Lambert {

public:

    /// Configure the StateSet with a GLSL program implementing the Lambert shader
    static void Lambert::configureGLSLProgram(const MObject &surface_shader, 
                                              TexturingConfig &texturing_config,
                                              osg::StateSet &state_set);

private:

    /**
     *	Build the vertex shader GLSL source code for Lambert materials
     *
     *	@param num_tc_sets	Number of texture coordinate sets used by this shader
     */
    static std::string getVertexShaderSrc( TexturingConfig &texturing_config );

    /// Build the fragment shader GLSL source code for Lambert materials
    static std::string getFragmentShaderSrc( osg::StateSet &state_set, 
                                             TexturingConfig &texturing_config,
                                             GLSLCodeBlock *color_cb,
                                             GLSLCodeBlock *ambient_cb,
                                             GLSLCodeBlock *emissive_cb );

};

#endif //_LAMBERT_H_
