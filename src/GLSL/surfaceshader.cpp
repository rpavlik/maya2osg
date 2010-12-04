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
#include "surfaceshader.h"
#include "shadingnetwork.h"

#include "../common.h"
#include "../config.h"
#include "../shader.h"

#include <maya/MFnDependencyNode.h>

#include <sstream>


/**
 *
 */
SurfaceShader::SurfaceShader( const MObject &shading_node, ShadingNetwork &shading_network ) :
    ShadingNode(shading_node, shading_network )
{
}


/**
 * Check whether this shader uses transparency
 */
bool SurfaceShader::hasTransparency()
{
    MFnDependencyNode dn(_mayaShadingNode);
    return Shader::connectedChannel(_mayaShadingNode, "transparency")
		|| dn.findPlug("transparencyR").asFloat() > 0.0
		|| dn.findPlug("transparencyG").asFloat() > 0.0
		|| dn.findPlug("transparencyB").asFloat() > 0.0;
}


/**
 *  Check whether this shader has a bump map connected
 */
bool SurfaceShader::hasBumpMap()
{
	if ( Config::instance()->getEnableBumpMapping() )
	    return Shader::connectedChannel(_mayaShadingNode, "normalCamera");
	else
		return false;
}


/**
 *  Get the vertex shader GLSL source code
 *
 *  This code is used when this surface shader is the root of a shading network,
 *  not when it is used as a shading node inside the shading network
 */
std::string SurfaceShader::getVertexShaderSrc()
{
    bool bump_map = hasBumpMap();

    std::string shader_src =
"// Exported with Maya2OSG " VERSION "\n"
"// http://maya2osg.sourceforge.net\n"
"#version 120\n"
"\n";
    if ( bump_map ) {
        shader_src += 
"attribute vec3 inTangent;\n";
    }
    shader_src +=
"varying vec3 ecPosition3;\n"
"varying vec3 normal;\n";
    if ( bump_map ) {
        shader_src += 
"varying vec3 tangent;\n"
"varying vec3 binormal;\n"
;
    }

    shader_src +=
"\n"
"void main() {\n"
"	// Transform vertex from object space to clip space\n"
"	gl_Position = ftransform();\n"
"\n"
"	// NOTE: Lighting computations are performed in eye coordinates\n"
"	// because OpenGL specifies that light positions are transformed\n"
"	// by the modelview matrix when they are provided to OpenGL\n"
"\n"
"	// Vertex position in eye coordinates (and projected)\n"
"	vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;\n"
"	ecPosition3 = (vec3(ecPosition)) / ecPosition.w;\n"
"\n"
"	// Normal (in eye coordinates)\n"
"	normal = normalize(gl_NormalMatrix * gl_Normal);\n";

    if ( bump_map ) {
        shader_src +=
"    tangent = normalize(gl_NormalMatrix * inTangent);\n"
"    binormal = cross(normal, tangent);\n"
;
    }

    shader_src +=
"\n"
;
    for ( std::set<int>::const_iterator i = _shadingNetwork.getTexturingConfig().getTCSetsUnitsUsed().begin() ; 
            i != _shadingNetwork.getTexturingConfig().getTCSetsUnitsUsed().end() ; i++ ) 
    {
		std::stringstream tc_set;
		tc_set << *i;
		shader_src += 
"	gl_TexCoord[" + tc_set.str() + "] = gl_TextureMatrix[" + 
				tc_set.str() + "] * gl_MultiTexCoord" + tc_set.str() + ";\n"
;
	}
    shader_src +=
"}\n";

	return shader_src;
}


/**
 *  Get the fragment shader GLSL source code
 *
 *  This code is used when this surface shader is the root of a shading network,
 *  not when it is used as a shading node inside the shading network
 */
std::string SurfaceShader::getFragmentShaderSrc()
{
    SurfaceShader::CodeBlock cb_fragment_output = getCodeBlock("fragmentOutput");
    bool bump_map = hasBumpMap();

    std::string shader_src;
    shader_src =
"// Exported with Maya2OSG " VERSION "\n"
"// http://maya2osg.sourceforge.net\n"
"#version 120\n"
"\n"
"// Uniforms set from Maya2OSG exporter\n"
"uniform bool LocalViewer;\n"
"uniform int NumEnabledLights;\n"
"\n"
"// From vertex shader\n"
"varying vec3 ecPosition3;\n"
"varying vec3 normal;\n";

    if ( bump_map ) {
        shader_src += 
"varying vec3 tangent;\n"
"varying vec3 binormal;\n"
;
    }

    shader_src +=
"\n"
;
    // Declarations
    shader_src += cb_fragment_output.declarations;
    // Functions
    shader_src += cb_fragment_output.functions;

    // Main
    shader_src +=
"void main()\n"
"{\n"
;

    // inline code
    shader_src += cb_fragment_output.computeCode;

    // variable names
    shader_src += 
"    gl_FragColor = " + cb_fragment_output.accessCode + ";\n"
"}\n"
;
    return shader_src;
}
