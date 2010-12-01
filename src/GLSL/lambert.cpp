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

#include "lambert.h"
#include "glslutils.h"
#include "shadingnetwork.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>


/**
 *  Constructor
 */
Lambert::Lambert( const MObject &shading_node, ShadingNetwork &shading_network ) :
    SurfaceShader( shading_node, shading_network ) 
{
}


/**
 *  Get the GLSL Codeblock for a plug
 */
ShadingNode::CodeBlock Lambert::getCodeBlock( const std::string &plug_name )
{
    // Build the GLSL code ...
    CodeBlock code_block;

    // WARNING. It is important to differentiate between the 
    // GLSL variable name and the return value
    // E.g. 
    //  The GLSL variable name 
    //
    //      vec4 sn_lambert_lambert1_color = ...
    //
    //  will serve to at least two output plugs with different GLSL code.
    //  WARNING! This code is what is called variableName in the CodeBlock structure
    //
    //      sn_lambert1_lambert1_color.rgb  -> plug "outColor"
    //      1.0 - sn_lambert1_lambert1_color.a    -> plug "outTransparency"
    //
    //  The variable name is stored in the map for the shading node plug
    //  The same variable name can be shared between plugs
    //  The access code for each plug is what is exported in the CodeBlock

    std::string variable_name;
    MFnDependencyNode dn(_mayaShadingNode);

    // Check plug name and avoid duplicating code

    if ( plug_name == "fragmentOutput" ||
         plug_name == "outColor" || 
         plug_name == "outTransparency" ) 
    {
        variable_name = "sn_lambert_" + std::string(dn.name().asChar()) + "_output";
        // Both color and transparency are stored in "color" variable.
        // if it is already declared, we omit all the GLSL code
        if ( !variableIsAvailable(variable_name) ) {

            // Supported Lambert input channels

            // Color (diffuse)
            Plug plug_color = getPlug("color");
            // Transparency
            Plug plug_transparency = getPlug("transparency");
            // Ambient Color
            Plug plug_ambient = getPlug("ambientColor");
            // Incandescence
            Plug plug_incandescence = getPlug("incandescence");
            // Bump map
            Plug plug_normal_camera;
			if ( hasBumpMap() ) {
				plug_normal_camera = getPlug("normalCamera");
			}
            // Diffuse (scalar)
            Plug plug_diffuse = getPlug("diffuse");

            code_block.declarations = plug_color.codeBlock.declarations +
                                      plug_transparency.codeBlock.declarations +
                                      plug_ambient.codeBlock.declarations +
                                      plug_incandescence.codeBlock.declarations +
                                      plug_normal_camera.codeBlock.declarations +
                                      plug_diffuse.codeBlock.declarations;

            code_block.functions = plug_color.codeBlock.functions +
                                   plug_transparency.codeBlock.functions +
                                   plug_ambient.codeBlock.functions +
                                   plug_incandescence.codeBlock.functions +
                                   plug_normal_camera.codeBlock.functions +
                                   plug_diffuse.codeBlock.functions +
                                   getDirectionalLightFunction() +
                                   getPointLightFunction() +
                                   getSpotLightFunction();

            code_block.computeCode = plug_color.codeBlock.computeCode +
                                     plug_transparency.codeBlock.computeCode +
                                     plug_ambient.codeBlock.computeCode +
                                     plug_incandescence.codeBlock.computeCode +
                                     plug_normal_camera.codeBlock.computeCode +
                                     plug_diffuse.codeBlock.computeCode +
"\n" +
			GLSLUtils::backFaceCulling(_shadingNetwork.getStateSet());

	        // Bump mapping
            if ( hasBumpMap() ) {
                code_block.computeCode +=
"    nnormal = " + plug_normal_camera.codeBlock.accessCode + ";\n"
;
            }

            code_block.computeCode +=
"	vec3 eye;\n"
"	if (LocalViewer)\n"
"	    eye = -normalize(ecPosition3);\n"
"	else\n"
"	    eye = vec3(0.0, 0.0, 1.0);\n"
"\n"
"	// Clear the light intensity accumulators\n"
"	vec4 amb  = vec4(0.0);\n"
"	vec4 diff = vec4(0.0);\n"
"\n"
"	// Loop through enabled lights, compute contribution from each\n"
"	int i;\n"
"	for (i = 0; i < NumEnabledLights; i++)\n"
"	{\n"
"	    if (gl_LightSource[i].position.w == 0.0) {\n"
"           vec3 lightDir = normalize(vec3(gl_LightSource[i].position));\n"
"	        DirectionalLight(nnormal, \n"
"                            lightDir, \n"
"                            gl_LightSource[i].ambient, \n"
"                            gl_LightSource[i].diffuse, \n"
"                            amb, diff);\n"
"       }\n"
"	    else if (gl_LightSource[i].spotCutoff == 180.0) {\n"
"	        PointLight(ecPosition3, \n"
"                      nnormal, \n"
"                      eye, \n"
"                      gl_LightSource[i].position.xyz, \n"
"                      vec3( gl_LightSource[i].constantAttenuation, \n"
"                            gl_LightSource[i].linearAttenuation, \n"
"                            gl_LightSource[i].quadraticAttenuation ), \n"
"                      gl_LightSource[i].ambient, \n"
"                      gl_LightSource[i].diffuse, \n"
"                      amb, \n"
"                      diff);\n"
"       }\n"
"	    else {\n"
"	        SpotLight(ecPosition3, \n"
"                     nnormal, \n"
"                     eye, \n"
"                     gl_LightSource[i].position.xyz, \n"
"                     vec3( gl_LightSource[i].constantAttenuation, \n"
"                           gl_LightSource[i].linearAttenuation, \n"
"                           gl_LightSource[i].quadraticAttenuation ), \n"
"                     gl_LightSource[i].spotDirection, \n"
"                     gl_LightSource[i].spotCosCutoff, \n"
"                     gl_LightSource[i].spotExponent, \n"
"                     gl_LightSource[i].ambient, \n"
"                     gl_LightSource[i].diffuse, \n"
"                     amb, \n"
"                     diff);\n"
"       }\n"
"	}\n"
"\n"
            // Channel : Diffuse color
"    vec3 diffuse_color = " + getPlugValue(plug_color) + " * " + getPlugValue(plug_diffuse) + ";\n"
            // Channel : Ambient color
"    vec3 ambient_color = " + getPlugValue(plug_ambient) + ";\n"
            // Channel : Emissive color
"    vec3 emissive_color = " + getPlugValue(plug_incandescence) + ";\n"
"\n";

    code_block.computeCode +=
"    vec4 " + variable_name + " = vec4( emissive_color +\n"
"                (amb.rgb + gl_LightModel.ambient.rgb) * ambient_color +\n"
"                diff.rgb * diffuse_color, 1.0 - " + getPlugValue(plug_transparency) + " );\n";

            _computedVariables.insert(variable_name);
        }
        // Access code
        if ( plug_name == "fragmentOutput" ) {  // special code for fragment output (RGBA)
            code_block.accessCode = variable_name;
        }
        else if ( plug_name == "outColor" ) {
            code_block.accessCode = variable_name + ".rgb";
        }
        else if ( plug_name == "outTransparency" ) {
            code_block.accessCode = "(1.0 - " + variable_name + ".a)";
        }
    }
    else {
        std::cerr << "WARNING: Unsupported plug " << plug_name << " in Lambert shader " << dn.name().asChar() << std::endl;
        // throw exception ??? !!! FIXME!!!
    }

    return code_block;
}


/**
 *	GLSL function for computing the contribution of a directional light
 */
std::string Lambert::getDirectionalLightFunction()
{
	std::string shader_src =
"void DirectionalLight(in vec3 normal,\n"
"                      in vec3 lightDir,\n"
"                      in vec4 lightAmbient,\n"
"                      in vec4 lightDiffuse,\n"
"                      inout vec4 ambient,\n"
"                      inout vec4 diffuse)\n"
"{\n"
"    float NdotL;         // normal . light direction\n"
"\n"
"    NdotL = max(0.0, dot(normal,\n"
"                 lightDir));\n"
"\n"
"    ambient  += lightAmbient;\n"
"    diffuse  += lightDiffuse * NdotL;\n"
"}\n"
;
	return shader_src;
}


/**
 *	GLSL function for computing the contribution of a point light
 */
std::string Lambert::getPointLightFunction()
{
    std::string shader_src = 
"void PointLight(in vec3 surfacePos,\n"
"                in vec3 normal,\n"
"                in vec3 eyePos,\n"
"                in vec3 lightPos,\n"
"                in vec3 lightAttenuation,\n"
"                in vec4 lightAmbient,\n"
"                in vec4 lightDiffuse,\n"
"                inout vec4 ambient,\n"
"                inout vec4 diffuse)\n"
"{\n"
"    float NdotL;          // normal . light direction\n"
"    float attenuation;    // computed attenuation factor\n"
"    float d;              // distance from surface to light source\n"
"    vec3 lightDir;        // direction from surface to light position\n"
"\n"
"    // Compute vector from surface to light position\n"
"    lightDir = vec3(lightPos) - surfacePos;\n"
"\n"
"    // Compute distance between surface and light position\n"
"    d = length(lightDir);\n"
"\n"
"    // Normalize the vector from surface to light position\n"
"    lightDir = normalize(lightDir);\n"
"\n"
"    // Compute attenuation\n"
"    attenuation = 1.0 / (lightAttenuation.x +\n"
"                         lightAttenuation.y * d +\n"
"                         lightAttenuation.z * d * d);\n"
"\n"
"    NdotL = max(0.0, dot(normal, lightDir));\n"
"\n"
"    ambient += lightAmbient * attenuation;\n"
"    diffuse += lightDiffuse * NdotL * attenuation;\n"
"}\n";
    return shader_src;
}


/**
 *	GLSL function for computing the contribution of a spot light
 */
std::string Lambert::getSpotLightFunction()
{
    std::string shader_src = 
"void SpotLight(in vec3 surfacePos,\n"
"               in vec3 normal,\n"
"               in vec3 eyePos,\n"
"               in vec3 lightPos,\n"
"               in vec3 lightAttenuation,\n"
"               in vec3 spotDirection,\n"
"               in float spotCosCutoff,\n"
"               in float spotExponent,\n"
"               in vec4 lightAmbient,\n"
"               in vec4 lightDiffuse,\n"
"               inout vec4 ambient,\n"
"               inout vec4 diffuse)\n"
"{\n"
"    float NdotL;            // normal . light direction\n"
"    float spotDot;          // cosine of angle between spotlight\n"
"    float spotAttenuation;  // spotlight attenuation factor\n"
"    float attenuation;      // computed attenuation factor\n"
"    float d;                // distance from surface to light source\n"
"    vec3 lightDir;          // direction from surface to light position\n"
"\n"
"    // Compute vector from surface to light position\n"
"    lightDir = lightPos - surfacePos;\n"
"\n"
"    // Compute distance between surface and light position\n"
"    d = length(lightDir);\n"
"\n"
"    // Normalize the vector from surface to light position\n"
"    lightDir = normalize(lightDir);\n"
"\n"
"    // Compute attenuation\n"
"    attenuation = 1.0 / (lightAttenuation.x +\n"
"                         lightAttenuation.y * d +\n"
"                         lightAttenuation.z * d * d);\n"
"\n"
"    // See if point on surface is inside cone of illumination\n"
"    spotDot = dot(-lightDir, normalize(spotDirection));\n"
"\n"
"    if (spotDot < spotCosCutoff)\n"
"        spotAttenuation = 0.0; // light adds no contribution\n"
"    else\n"
"        spotAttenuation = pow(spotDot, spotExponent);\n"
"\n"
"    // Combine the spotlight and distance attenuation.\n"
"    attenuation *= spotAttenuation;\n"
"\n"
"    NdotL = max(0.0, dot(normal, lightDir));\n"
"\n"
"    ambient  += lightAmbient * attenuation;\n"
"    diffuse  += lightDiffuse * NdotL * attenuation;\n"
"}\n"
;
    return shader_src;
}
