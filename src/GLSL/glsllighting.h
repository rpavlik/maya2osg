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
#ifndef _GLSLLIGHTING_H_
#define _GLSLLIGHTING_H_

#include <string>

/**
 *	Support for lighting computation in GLSL shaders
 *
 *  NOTE: See Orange Book chapter 9 (Emulating OpenGL Fixed Functionality)
 */
class GLSLLighting {

public:

    /**
     *  Return the GLSL source code for the directional light computation
     *
     *  void DirectionalLight(in int i,
     *                        in vec3 normal,
     *                        inout vec4 ambient,
     *                        inout vec4 diffuse,
     *                        inout vec4 specular)
     */
    static std::string getDirectionalLightFunction();

    /**
     *  Return the GLSL source code for the directional light computation
     *  without the specular component
     *
     *  void DirectionalLight(in int i,
     *                        in vec3 normal,
     *                        inout vec4 ambient,
     *                        inout vec4 diffuse)
     */
    static std::string getDirectionalLightFunctionWithoutSpecular();

    /**
     *  Return the GLSL source code for the point light computation
     *
     *  void PointLight(in int i,
     *                  in vec3 eye,
     *                  in vec3 ecPosition3,
     *                  in vec3 normal,
     *                  inout vec4 ambient,
     *                  inout vec4 diffuse,
     *                  inout vec4 specular)
     */
    static std::string getPointLightFunction();

    /**
     *  Return the GLSL source code for the point light computation
     *  without the specular component
     *
     *  void PointLight(in int i,
     *                  in vec3 eye,
     *                  in vec3 ecPosition3,
     *                  in vec3 normal,
     *                  inout vec4 ambient,
     *                  inout vec4 diffuse)
     */
    static std::string getPointLightFunctionWithoutSpecular();

    /**
     *  Return the GLSL source code for the spot light computation
     *
     *  void SpotLight(in int i,
     *                  in vec3 eye,
     *                  in vec3 ecPosition3,
     *                  in vec3 normal,
     *                  inout vec4 ambient,
     *                  inout vec4 diffuse,
     *                  inout vec4 specular)
     */
    static std::string getSpotLightFunction();

    /**
     *  Return the GLSL source code for the spot light computation
     *  without the specular component
     *
     *  void SpotLight(in int i,
     *                  in vec3 eye,
     *                  in vec3 ecPosition3,
     *                  in vec3 normal,
     *                  inout vec4 ambient,
     *                  inout vec4 diffuse)
     */
    static std::string getSpotLightFunctionWithoutSpecular();

    // TO-DO: Bump-mapping support functions

};

#endif //_GLSLLIGHTING_H_
