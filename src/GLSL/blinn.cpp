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

#include "blinn.h"
#include "glslutils.h"
#include "shadingnetwork.h"
#include "../common.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

#define DISCARD_SELF_SHADOWED_BEFORE_BUMP_MAPPING


/**
 *  Constructor
 */
Blinn::Blinn( const MObject &shading_node, ShadingNetwork &shading_network ) :
    SurfaceShader( shading_node, shading_network ) 
{
}


/**
 *  Get the GLSL Codeblock for a plug
 */
ShadingNode::CodeBlock Blinn::getCodeBlock( const std::string &plug_name )
{
    // Build the GLSL code ...
    CodeBlock code_block;

    std::string variable_name;
    MFnDependencyNode dn(_mayaShadingNode);
    std::string maya_blinn_shader_name = dn.name().asChar();
    hygienizeName( maya_blinn_shader_name );

    // Check plug name and avoid duplicating code

    if ( plug_name == "fragmentOutput" ||
         plug_name == "outColor" || 
         plug_name == "outTransparency" ) 
    {
        variable_name = "sn_blinn_" + maya_blinn_shader_name + "_output";
        // Both color and transparency are stored in "color" variable.
        // if it is already declared, we omit all the GLSL code
        if ( !variableIsAvailable(variable_name) ) {

            // Supported Blinn input channels

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
            // Eccentricity
            Plug plug_eccentricity = getPlug("eccentricity");
            // Specular Rolloff
            Plug plug_specular_rolloff = getPlug("specularRollOff");
            // Specular Color
            Plug plug_specular_color = getPlug("specularColor");

            // TO-DO ... Reflectivity, Reflected Color

            code_block.declarations = plug_color.codeBlock.declarations +
                                      plug_transparency.codeBlock.declarations +
                                      plug_ambient.codeBlock.declarations +
                                      plug_incandescence.codeBlock.declarations +
                                      plug_normal_camera.codeBlock.declarations +
                                      plug_diffuse.codeBlock.declarations + 
                                      plug_eccentricity.codeBlock.declarations +
                                      plug_specular_rolloff.codeBlock.declarations +
                                      plug_specular_color.codeBlock.declarations;

            code_block.functions = plug_color.codeBlock.functions +
                                   plug_transparency.codeBlock.functions +
                                   plug_ambient.codeBlock.functions +
                                   plug_incandescence.codeBlock.functions +
                                   plug_normal_camera.codeBlock.functions +
                                   plug_diffuse.codeBlock.functions +
                                   plug_eccentricity.codeBlock.functions +
                                   plug_specular_rolloff.codeBlock.functions +
                                   plug_specular_color.codeBlock.functions +
                                   getDirectionalLightFunction() +
                                   getPointLightFunction() +
                                   getSpotLightFunction();

            code_block.computeCode = plug_color.codeBlock.computeCode +
                                     plug_transparency.codeBlock.computeCode +
                                     plug_ambient.codeBlock.computeCode +
                                     plug_incandescence.codeBlock.computeCode +
                                     plug_normal_camera.codeBlock.computeCode +
                                     plug_diffuse.codeBlock.computeCode +
                                     plug_eccentricity.codeBlock.computeCode +
                                     plug_specular_rolloff.codeBlock.computeCode +
                                     plug_specular_color.codeBlock.computeCode +
"\n" +
            GLSLUtils::backFaceCulling(_shadingNetwork.getStateSet());

            code_block.computeCode +=
"    // Clear the light intensity accumulators\n"
"    vec4 amb  = vec4(0.0);\n"
"    vec4 diff = vec4(0.0);\n"
"   vec4 spec = vec4(0.0);\n"
"\n"
"    // Loop through enabled lights, compute contribution from each\n"
"    int i;\n"
"    for (i = 0; i < NumEnabledLights; i++)\n"
"    {\n"
"        if (gl_LightSource[i].position.w == 0.0) {\n";
            // ----- DIRECTIONAL LIGHT
            if ( hasBumpMap() ) {
                code_block.computeCode +=
#ifdef DISCARD_SELF_SHADOWED_BEFORE_BUMP_MAPPING
"           if ( dot( nnormal, normalize(gl_LightSource[i].position.xyz) ) > 0.0 )\n"
#endif
"           {\n"
"               vec3 lightDir;\n"
"               lightDir.x = dot(gl_LightSource[i].position.xyz, tangent);\n"
"               lightDir.y = dot(gl_LightSource[i].position.xyz, binormal);\n"
"               lightDir.z = dot(gl_LightSource[i].position.xyz, normal);\n"
"               vec3 halfVector;\n"
"               halfVector.x = dot(gl_LightSource[i].halfVector.xyz, tangent);\n"
"               halfVector.y = dot(gl_LightSource[i].halfVector.xyz, binormal);\n"
"               halfVector.z = dot(gl_LightSource[i].halfVector.xyz, normal);\n"
"                DirectionalLight(" + plug_normal_camera.codeBlock.accessCode + ", \n"
"                                normalize(lightDir), \n"
"                                normalize(halfVector), \n"
"                                gl_LightSource[i].ambient, \n"
"                                gl_LightSource[i].diffuse, \n"
"                                gl_LightSource[i].specular, \n"
"                                gl_FrontMaterial.shininess, \n"
"                                amb, diff, spec); \n"
"           }\n";
            }
            else {
                code_block.computeCode +=
"            DirectionalLight(nnormal, \n"
"                            normalize(vec3(gl_LightSource[i].position)), \n"
"                            gl_LightSource[i].halfVector.xyz,\n"
"                            gl_LightSource[i].ambient, \n"
"                            gl_LightSource[i].diffuse, \n"
"                            gl_LightSource[i].specular, \n"
"                            gl_FrontMaterial.shininess, \n"
"                            amb, diff, spec);\n";
            }
            code_block.computeCode +=
"       }\n"
"        else if (gl_LightSource[i].spotCutoff == 180.0) {\n";
            // ----- POINT LIGHT
            if ( hasBumpMap() ) {
                code_block.computeCode +=
#ifdef DISCARD_SELF_SHADOWED_BEFORE_BUMP_MAPPING
"           if ( dot( nnormal, normalize(gl_LightSource[i].position.xyz - ecPosition3) ) > 0.0 )\n"
#endif
"           {\n"
"               vec3 lightPos;\n"
"               lightPos.x = dot(gl_LightSource[i].position.xyz, tangent);\n"
"               lightPos.y = dot(gl_LightSource[i].position.xyz, binormal);\n"
"               lightPos.z = dot(gl_LightSource[i].position.xyz, normal);\n"
"               vec3 surfPos;\n"
"               surfPos.x = dot(ecPosition3, tangent);\n"
"               surfPos.y = dot(ecPosition3, binormal);\n"
"               surfPos.z = dot(ecPosition3, normal);\n"
"               PointLight(surfPos, \n"
"                          " + plug_normal_camera.codeBlock.accessCode + ",\n"
"                          lightPos, \n"
"                          vec3( gl_LightSource[i].constantAttenuation, \n"
"                                gl_LightSource[i].linearAttenuation, \n"
"                                gl_LightSource[i].quadraticAttenuation ), \n"
"                          gl_LightSource[i].ambient, \n"
"                          gl_LightSource[i].diffuse, \n"
"                          gl_LightSource[i].specular, \n"
"                          gl_FrontMaterial.shininess, \n"
"                          amb, \n"
"                          diff, \n"
"                          spec);\n"
"           }\n";
            }
            else {
                code_block.computeCode +=
"            PointLight(ecPosition3, \n"
"                      nnormal, \n"
"                      gl_LightSource[i].position.xyz, \n"
"                      vec3( gl_LightSource[i].constantAttenuation, \n"
"                            gl_LightSource[i].linearAttenuation, \n"
"                            gl_LightSource[i].quadraticAttenuation ), \n"
"                      gl_LightSource[i].ambient, \n"
"                      gl_LightSource[i].diffuse, \n"
"                      gl_LightSource[i].specular, \n"
"                      gl_FrontMaterial.shininess, \n"
"                      amb, \n"
"                      diff, \n"
"                      spec);\n";
            }
            code_block.computeCode +=
"       }\n"
"        else {\n";
            // ----- SPOT LIGHT
            if ( hasBumpMap() ) {
                code_block.computeCode +=
#ifdef DISCARD_SELF_SHADOWED_BEFORE_BUMP_MAPPING
"           if ( dot( nnormal, normalize(gl_LightSource[i].position.xyz - ecPosition3) ) > 0.0 )\n"
#endif
"           {\n"
"               vec3 lightPos;\n"
"               lightPos.x = dot(gl_LightSource[i].position.xyz, tangent);\n"
"               lightPos.y = dot(gl_LightSource[i].position.xyz, binormal);\n"
"               lightPos.z = dot(gl_LightSource[i].position.xyz, normal);\n"
"               vec3 surfPos;\n"
"               surfPos.x = dot(ecPosition3, tangent);\n"
"               surfPos.y = dot(ecPosition3, binormal);\n"
"               surfPos.z = dot(ecPosition3, normal);\n"
"               vec3 spotDir;\n"
"               spotDir.x = dot(gl_LightSource[i].spotDirection, tangent);\n"
"               spotDir.y = dot(gl_LightSource[i].spotDirection, binormal);\n"
"               spotDir.z = dot(gl_LightSource[i].spotDirection, normal);\n"
"               SpotLight(surfPos, \n"
"                         " + plug_normal_camera.codeBlock.accessCode + ",\n"
"                         lightPos, \n"
"                         vec3( gl_LightSource[i].constantAttenuation, \n"
"                               gl_LightSource[i].linearAttenuation, \n"
"                               gl_LightSource[i].quadraticAttenuation ), \n"
"                         spotDir, \n"
"                         gl_LightSource[i].spotCosCutoff, \n"
"                         gl_LightSource[i].spotExponent, \n"
"                         gl_LightSource[i].ambient, \n"
"                         gl_LightSource[i].diffuse, \n"
"                         gl_LightSource[i].specular, \n"
"                         gl_FrontMaterial.shininess, \n"
"                         amb, \n"
"                         diff, \n"
"                         spec);\n"
"           }\n";
            }
            else {
                code_block.computeCode +=
"            SpotLight(ecPosition3, \n"
"                     nnormal, \n"
"                     gl_LightSource[i].position.xyz, \n"
"                     vec3( gl_LightSource[i].constantAttenuation, \n"
"                           gl_LightSource[i].linearAttenuation, \n"
"                           gl_LightSource[i].quadraticAttenuation ), \n"
"                     gl_LightSource[i].spotDirection, \n"
"                     gl_LightSource[i].spotCosCutoff, \n"
"                     gl_LightSource[i].spotExponent, \n"
"                     gl_LightSource[i].ambient, \n"
"                     gl_LightSource[i].diffuse, \n"
"                     gl_LightSource[i].specular, \n"
"                     gl_FrontMaterial.shininess, \n"
"                     amb, \n"
"                     diff, \n"
"                     spec);\n";
            }
            code_block.computeCode +=
"       }\n"
"    }\n"
"\n"
            // Channel : Diffuse color
"    vec3 diffuse_color = " + getPlugValue(plug_color) + " * " + getPlugValue(plug_diffuse) + ";\n"
            // Channel : Ambient color
"    vec3 ambient_color = " + getPlugValue(plug_ambient) + ";\n"
            // Channel : Emissive color
"    vec3 emissive_color = " + getPlugValue(plug_incandescence) + ";\n"
            // Channel : Specular color
"    vec3 specular_color = " + getPlugValue(plug_specular_color) + ";\n"
"\n";

    code_block.computeCode +=
"    vec4 " + variable_name + " = vec4( emissive_color +\n"
"                (amb.rgb + gl_LightModel.ambient.rgb) * ambient_color +\n"
"                 diff.rgb * diffuse_color +\n"
"                 spec.rgb * specular_color, 1.0 - " + getPlugValue(plug_transparency) + " );\n";

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
 *    GLSL function for computing the contribution of a directional light
 */
std::string Blinn::getDirectionalLightFunction()
{
    std::string shader_src =
"void DirectionalLight(in vec3 normal,\n"
"                      in vec3 lightDir,\n"
"                      in vec3 halfVector,\n"
"                      in vec4 lightAmbient,\n"
"                      in vec4 lightDiffuse,\n"
"                      in vec4 lightSpecular,\n"
"                      in float shininess,\n"
"                      inout vec4 ambient,\n"
"                      inout vec4 diffuse,\n"
"                      inout vec4 specular)\n"
"{\n"
"    float NdotL;         // normal . light direction\n"
"    float NdotHV;        // normal . half vector\n"
"    float pf;            // power factor\n"
"\n"
"    NdotL = max(0.0, dot(normal,\n"
"                 lightDir));\n"
"    NdotHV = max(0.0, dot(normal, \n"
"               halfVector));\n"
"\n"
"    if ( NdotHV == 0.0 )\n"
"        pf = 0.0;\n"
"    else\n"
"        pf = pow(NdotHV, shininess);\n"
"\n"
"    ambient  += lightAmbient;\n"
"    diffuse  += lightDiffuse * NdotL;\n"
"    specular += lightSpecular * pf;\n"
"}\n"
;
    return shader_src;
}


/**
 *    GLSL function for computing the contribution of a point light
 */
std::string Blinn::getPointLightFunction()
{
    std::string shader_src = 
"void PointLight(in vec3 surfacePos,\n"
"                in vec3 normal,\n"
"                in vec3 lightPos,\n"
"                in vec3 lightAttenuation,\n"
"                in vec4 lightAmbient,\n"
"                in vec4 lightDiffuse,\n"
"                in vec4 lightSpecular,\n"
"                in float shininess,\n"
"                inout vec4 ambient,\n"
"                inout vec4 diffuse,\n"
"                inout vec4 specular)\n"
"{\n"
"    float NdotL;          // normal . light direction\n"
"    float NdotHV;         // normal . half vector\n"
"    float pf;             // power factor\n"
"    float attenuation;    // computed attenuation factor\n"
"    float d;              // distance from surface to light source\n"
"    vec3 lightDir;        // direction from surface to light position\n"
"    vec3 halfVector;      // direction of maximum highlights\n"
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
"    halfVector = normalize(lightDir - normalize(surfacePos));\n"
"\n"
"    NdotL = max(0.0, dot(normal, lightDir));\n"
"    NdotHV = max(0.0, dot(normal, halfVector));\n"
"\n"
"    if ( NdotL == 0.0 )\n"
"        pf = 0.0;\n"
"    else\n"
"        pf = pow(NdotHV, shininess);\n"
"\n"
"    ambient += lightAmbient * attenuation;\n"
"    diffuse += lightDiffuse * NdotL * attenuation;\n"
"    specular += lightSpecular * pf * attenuation;\n"
"}\n";
    return shader_src;
}


/**
 *    GLSL function for computing the contribution of a spot light
 */
std::string Blinn::getSpotLightFunction()
{
    std::string shader_src = 
"void SpotLight(in vec3 surfacePos,\n"
"               in vec3 normal,\n"
"               in vec3 lightPos,\n"
"               in vec3 lightAttenuation,\n"
"               in vec3 spotDirection,\n"
"               in float spotCosCutoff,\n"
"               in float spotExponent,\n"
"               in vec4 lightAmbient,\n"
"               in vec4 lightDiffuse,\n"
"               in vec4 lightSpecular,\n"
"               in float shininess,\n"
"               inout vec4 ambient,\n"
"               inout vec4 diffuse,\n"
"               inout vec4 specular)\n"
"{\n"
"    float NdotL;            // normal . light direction\n"
"    float NdotHV;           // normal . half vector\n"
"    float pf;               // power factor\n"
"    float spotDot;          // cosine of angle between spotlight\n"
"    float spotAttenuation;  // spotlight attenuation factor\n"
"    float attenuation;      // computed attenuation factor\n"
"    float d;                // distance from surface to light source\n"
"    vec3 lightDir;          // direction from surface to light position\n"
"    vec3 halfVector;        // direction of maximum highlights\n"
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
"    halfVector = normalize(lightDir - normalize(surfacePos));\n"
"\n"
"    NdotL = max(0.0, dot(normal, lightDir));\n"
"    NdotHV = max(0.0, dot(normal, halfVector));\n"
"\n"
"    if ( NdotL == 0.0 )\n"
"        pf = 0.0;\n"
"    else\n"
"        pf = pow(NdotHV, shininess);\n"
"\n"
"    ambient  += lightAmbient * attenuation;\n"
"    diffuse  += lightDiffuse * NdotL * attenuation;\n"
"    specular += lightSpecular * pf * attenuation;\n"
"}\n"
;
    return shader_src;
}
