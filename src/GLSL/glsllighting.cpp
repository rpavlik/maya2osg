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

#include "glsllighting.h"

/**
 *  Return the GLSL source code for the directional light computation
 *
 *  void DirectionalLight(in int i,
 *                        in vec3 normal,
 *                        inout vec4 ambient,
 *                        inout vec4 diffuse,
 *                        inout vec4 specular)
 */
std::string GLSLLighting::getDirectionalLightFunction()
{
	std::string shader_src =
"void DirectionalLight(in int i,\n"
"                      in vec3 normal,\n"
"                      inout vec4 ambient,\n"
"                      inout vec4 diffuse,\n"
"                      inout vec4 specular)\n"
"{\n"
"    float nDotVP;         // normal . light direction\n"
"    float nDotHV;         // normal . light half vector\n"
"    float pf;             // power factor\n"
"\n"
"    nDotVP = max(0.0, dot(normal,\n"
"                 normalize(vec3(gl_LightSource[i].position))));\n"
"    nDotHV = max(0.0, dot(normal,vec3(gl_LightSource[i].halfVector)));\n"
"\n"
"    if ( nDotVP == 0.0 )\n"
"        pf = 0.0;\n"
"    else\n"
"        pf = pow(nDotHV, gl_FrontMaterial.shininess);\n"
"\n"
"    ambient  += gl_LightSource[i].ambient;\n"
"    diffuse  += gl_LightSource[i].diffuse * nDotVP;\n"
"    specular += gl_LightSource[i].specular * pf;\n"
"}\n"
;
	return shader_src;
}


/**
 *  Return the GLSL source code for the directional light computation
 *  without the specular component
 *
 *  void DirectionalLight(in int i,
 *                        in vec3 normal,
 *                        inout vec4 ambient,
 *                        inout vec4 diffuse)
 */
std::string GLSLLighting::getDirectionalLightFunctionWithoutSpecular()
{
	std::string shader_src =
"void DirectionalLight(in int i,\n"
"                      in vec3 normal,\n"
"                      inout vec4 ambient,\n"
"                      inout vec4 diffuse)\n"
"{\n"
"    float nDotVP;         // normal . light direction\n"
"\n"
"    nDotVP = max(0.0, dot(normal,\n"
"                 normalize(vec3(gl_LightSource[i].position))));\n"
"\n"
"    ambient  += gl_LightSource[i].ambient;\n"
"    diffuse  += gl_LightSource[i].diffuse * nDotVP;\n"
"}\n"
;
	return shader_src;
}


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
std::string GLSLLighting::getPointLightFunction()
{
    std::string shader_src = 
"void PointLight(in int i,\n"
"                in vec3 eye,\n"
"                in vec3 ecPosition3,\n"
"                in vec3 normal,\n"
"                inout vec4 ambient,\n"
"                inout vec4 diffuse,\n"
"                inout vec4 specular)\n"
"{\n"
"    float nDotVP;         // normal . light direction\n"
"    float nDotHP;         // normal . light half vector\n"
"    float pf;             // power factor\n"
"    float attenuation;    // computed attenuation factor\n"
"    float d;              // distance from surface to light source\n"
"    vec3  VP;             // direction from surface to light position\n"
"    halfVector;           // direction of maximum highlights\n"
"\n"
"    // Compute vector from surface to light position\n"
"    VP = vec3(gl_LightSource[i].position) - ecPosition3;\n"
"\n"
"    // Compute distance between surface and light position\n"
"    d = length(VP);\n"
"\n"
"    // Normalize the vector from surface to light position\n"
"    VP = normalize(VP);\n"
"\n"
"    // Compute attenuation\n"
"    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +\n"
"                         gl_LightSource[i].linearAttenuation * d +\n"
"                         gl_LightSource[i].quadraticAttenuation * d * d);\n"
"\n"
"    halfVector = normalize(VP + eye);\n"
"\n"
"    nDotVP = max(0.0, dot(normal, VP));\n"
"    nDotHV = max(0.0, dot(normal, halfVector));\n"
"\n"
"    if (nDotVP == 0.0)\n"
"        pf = 0.0;\n"
"    else\n"
"        pf = pow(nDotHV, gl_FrontMaterial.shininess);\n"
"\n"
"    ambient += gl_LightSource[i].ambient * attenuation;\n"
"    diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;\n"
"    specular += gl_LightSource[i].specular * pf * attenuation;\n"
"}\n";
    return shader_src;
}


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
std::string GLSLLighting::getPointLightFunctionWithoutSpecular()
{
    std::string shader_src = 
"void PointLight(in int i,\n"
"                in vec3 eye,\n"
"                in vec3 ecPosition3,\n"
"                in vec3 normal,\n"
"                inout vec4 ambient,\n"
"                inout vec4 diffuse)\n"
"{\n"
"    float nDotVP;         // normal . light direction\n"
"    float attenuation;    // computed attenuation factor\n"
"    float d;              // distance from surface to light source\n"
"    vec3  VP;             // direction from surface to light position\n"
"\n"
"    // Compute vector from surface to light position\n"
"    VP = vec3(gl_LightSource[i].position) - ecPosition3;\n"
"\n"
"    // Compute distance between surface and light position\n"
"    d = length(VP);\n"
"\n"
"    // Normalize the vector from surface to light position\n"
"    VP = normalize(VP);\n"
"\n"
"    // Compute attenuation\n"
"    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +\n"
"                         gl_LightSource[i].linearAttenuation * d +\n"
"                         gl_LightSource[i].quadraticAttenuation * d * d);\n"
"\n"
"    nDotVP = max(0.0, dot(normal, VP));\n"
"\n"
"    ambient += gl_LightSource[i].ambient * attenuation;\n"
"    diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;\n"
"}\n";
    return shader_src;
}


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
std::string GLSLLighting::getSpotLightFunction()
{
    std::string shader_src = 
"void SpotLight(in int i,\n"
"               in vec3 eye,\n"
"               in vec3 ecPosition3,\n"
"               in vec3 normal,\n"
"               inout vec4 ambient,\n"
"               inout vec4 diffuse\n"
"               inout vec4 specular)\n"
"{\n"
"    float nDotVP;           // normal . light direction\n"
"    float nDotHP;           // normal . light half vector\n"
"    float pf;               // power factor\n"
"    float spotDot;          // cosine of angle between spotlight\n"
"    float spotAttenuation;  // spotlight attenuation factor\n"
"    float attenuation;      // computed attenuation factor\n"
"    float d;                // distance from surface to light source\n"
"    vec3 VP;                // direction from surface to light position\n"
"    vec3 halfVector;        // direction of maximum highlights\n"
"\n"
"    // Compute vector from surface to light position\n"
"    VP = vec3(gl_LightSource[i].position) - ecPosition3;\n"
"\n"
"    // Compute distance between surface and light position\n"
"    d = length(VP);\n"
"\n"
"    // Normalize the vector from surface to light position\n"
"    VP = normalize(VP);\n"
"\n"
"    // Compute attenuation\n"
"    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +\n"
"                         gl_LightSource[i].linearAttenuation * d +\n"
"                         gl_LightSource[i].quadraticAttenuation * d * d);\n"
"\n"
"    // See if point on surface is inside cone of illumination\n"
"    spotDot = dot(-VP, normalize(gl_LightSource[i].spotDirection));\n"
"\n"
"    if (spotDot < gl_LightSource[i].spotCosCutoff)\n"
"        spotAttenuation = 0.0; // light adds no contribution\n"
"    else\n"
"        spotAttenuation = pow(spotDot, gl_LightSource[i].spotExponent);\n"
"\n"
"    // Combine the spotlight and distance attenuation.\n"
"    attenuation *= spotAttenuation;\n"
"\n"
"    halfVector = normalize(VP + eye);\n"
"\n"
"    nDotVP = max(0.0, dot(normal, VP));\n"
"    nDotHV = max(0.0, dot(normal, halfVector));\n"
"\n"
"    if(nDotVP == 0.0)\n"
"        pf = 0.0;\n"
"    else\n"
"        pf = pow(nDotHV, gl_FrontMaterial.shininess);\n"
"\n"
"    ambient  += gl_LightSource[i].ambient * attenuation;\n"
"    diffuse  += gl_LightSource[i].diffuse * nDotVP * attenuation;\n"
"    specular += gl_LightSource[i].specular * pf * attenuation;\n"
"}\n"
;
    return shader_src;
}


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
std::string GLSLLighting::getSpotLightFunctionWithoutSpecular()
{
    std::string shader_src = 
"void SpotLight(in int i,\n"
"               in vec3 eye,\n"
"               in vec3 ecPosition3,\n"
"               in vec3 normal,\n"
"               inout vec4 ambient,\n"
"               inout vec4 diffuse)\n"
"{\n"
"    float nDotVP;           // normal . light direction\n"
"    float spotDot;          // cosine of angle between spotlight\n"
"    float spotAttenuation;  // spotlight attenuation factor\n"
"    float attenuation;      // computed attenuation factor\n"
"    float d;                // distance from surface to light source\n"
"    vec3 VP;                // direction from surface to light position\n"
"\n"
"    // Compute vector from surface to light position\n"
"    VP = vec3(gl_LightSource[i].position) - ecPosition3;\n"
"\n"
"    // Compute distance between surface and light position\n"
"    d = length(VP);\n"
"\n"
"    // Normalize the vector from surface to light position\n"
"    VP = normalize(VP);\n"
"\n"
"    // Compute attenuation\n"
"    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +\n"
"                         gl_LightSource[i].linearAttenuation * d +\n"
"                         gl_LightSource[i].quadraticAttenuation * d * d);\n"
"\n"
"    // See if point on surface is inside cone of illumination\n"
"    spotDot = dot(-VP, normalize(gl_LightSource[i].spotDirection));\n"
"\n"
"    if (spotDot < gl_LightSource[i].spotCosCutoff)\n"
"        spotAttenuation = 0.0; // light adds no contribution\n"
"    else\n"
"        spotAttenuation = pow(spotDot, gl_LightSource[i].spotExponent);\n"
"\n"
"    // Combine the spotlight and distance attenuation.\n"
"    attenuation *= spotAttenuation;\n"
"\n"
"    nDotVP = max(0.0, dot(normal, VP));\n"
"\n"
"    ambient  += gl_LightSource[i].ambient * attenuation;\n"
"    diffuse  += gl_LightSource[i].diffuse * nDotVP * attenuation;\n"
"}\n"
;
    return shader_src;
}
