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
#include "shaderglsl.h"
#include "shader.h"
#include "config.h"
#include "lights.h"

#include <osg/BlendFunc>

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

#include <sstream>


////////////////////////////////////////////////////////////////////////////////

// NOTE: See Orange Book chapter 9 (Emulating OpenGL Fixed Functionality)

/**
 *	Build the vertex shader GLSL source code for Lambert materials
 *
 *	@param num_tc_sets	Number of texture coordinate sets used by this shader
 */
std::string LambertVertexShaderSrc( int num_tc_sets )
{
	std::string shader_src =
"// Texture units used\n"
"uniform int numTexCoordSets;\n"
"\n"
"varying vec3 ecPosition3;\n"
"varying vec3 normal;\n"
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
"	// Normals (in eye coordinates, we will normalize them after interpolation)\n"
"	normal = gl_NormalMatrix * gl_Normal;\n"
"\n"
;
	for ( int i=0 ; i < num_tc_sets ; i++ ) {
		std::stringstream tc_set;
		tc_set << i;
		shader_src += 
"	gl_TexCoord[" + tc_set.str() + "] = gl_TextureMatrix[" + 
				tc_set.str() + "] * gl_MultiTexCoord" + tc_set.str() + ";\n"
"}\n"
;
	}
	return shader_src;
}


/**
 *	Build the fragment shader GLSL source code for Lambert materials
 *
 *	@param num_tc_sets	Number of texture coordinate sets used by this shader
 */
std::string LambertFragmentShaderSrc()
{
	std::string shader_src =
"#version 120\n"
"\n"
"// Uniforms set from Maya2OSG exporter\n"
"uniform bool LocalViewer;\n"
"uniform int NumEnabledLights;\n"
"\n"
"// From vertex shader\n"
"varying vec3 ecPosition3;\n"
"varying vec3 normal;\n"
"\n"
"void DirectionalLight(in int i,\n"
"                      in vec3 normal,\n"
"                      inout vec4 ambient,\n"
"                      inout vec4 diffuse)\n"
"{\n"
"     float nDotVP;         // normal . light direction\n"
"     float pf;             // power factor\n"
"\n"
"     nDotVP = max(0.0, dot(normal,\n"
"                   normalize(vec3(gl_LightSource[i].position))));\n"
"\n"
"     ambient  += gl_LightSource[i].ambient;\n"
"     diffuse  += gl_LightSource[i].diffuse * nDotVP;\n"
"}\n"
"\n"
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
"}\n"
"\n"
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
"\n"
"\n"
"void main() {\n"
"\n"
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
"	    if (gl_LightSource[i].position.w == 0.0)\n"
"	        DirectionalLight(i, normalize(normal), amb, diff);\n"
"	    else if (gl_LightSource[i].spotCutoff == 180.0)\n"
"	        PointLight(i, eye, ecPosition3, normalize(normal), amb, diff);\n"
"	    else\n"
"	        SpotLight(i, eye, ecPosition3, normalize(normal), amb, diff);\n"
"	}\n"
"\n"
"	// NOTE: gl_FrontLightModelProduct.sceneColor = gl_FrontMaterial.emission + gl_FrontMaterial.ambient * gl_LightModel.ambient\n"
"	vec4 color = gl_FrontLightModelProduct.sceneColor +\n"
"	            amb * gl_FrontMaterial.ambient +\n"
"	            diff * gl_FrontMaterial.diffuse;\n"
"\n"
"	gl_FragColor = color;\n"
"}\n";
    return shader_src;
}


/**
 *	Configure the shaders for the StateSet of the ShadingEngine/ShadingGroup object
 */
void ShaderGLSL::exporta(const MObject &shading_engine, const Texture2UVSetMap &textures_map,
						int num_tc_sets, osg::ref_ptr<osg::StateSet> state_set)
{
	MObject surface_shader;
	Shader::getSurfaceShader( shading_engine, surface_shader );

	if ( surface_shader.hasFn(MFn::kLambert) ) {
		exportLambert( surface_shader, textures_map, num_tc_sets, state_set );
	}
	//else if ...
	// TO-DO : rest (non Lambert-derived) of Maya shaders *****  FIXME!!!
}


/**
 *	Create the stateset for a Lambert material
 *
 *	********* UNDER CONSTRUCTION!!!   --- FIXME!!!!!!!!
 */
void ShaderGLSL::exportLambert(const MObject &surface_shader, const Texture2UVSetMap &textures_map,
                               int num_tc_sets, osg::ref_ptr<osg::StateSet> state_set)
{
	MFnDependencyNode dn(surface_shader);

	// Check if there is transparency
	bool transparency = Shader::connectedChannel(surface_shader, "transparency") 
		|| dn.findPlug("transparencyR").asFloat() > 0.0
		|| dn.findPlug("transparencyG").asFloat() > 0.0
		|| dn.findPlug("transparencyB").asFloat() > 0.0;

	// Export common material attributes
	// FIXME *** -> this code will be common to every shader that inherit from lambert node,
	//		it will be put on a separate function/method

	// Color
	//	texture?
	
	// Transparency
	//	texture?
	// Ambient Color
	//	texture?
	// Incandescence
	//	texture?
	// Bump Mapping
	//	texture?
	// Diffuse

	// Translucence (*)
	// Translucence Depth (*)
	// Translucence Focus (*)

	// (*) Parameter currently ignored

	// GLSL Program
	osg::Program *program = new osg::Program();

    // Vertex shader
#if 0
	osg::Shader *vertex_shader = new osg::Shader(osg::Shader::VERTEX);
	vertex_shader->loadShaderSourceFromFile( "../../shaders/lambert_vertex.glsl" );
	program->addShader( vertex_shader );
#else
	program->addShader( new osg::Shader(osg::Shader::VERTEX, LambertVertexShaderSrc(num_tc_sets)) );
#endif

    // Fragment shader
#if 1
	osg::Shader *fragment_shader = new osg::Shader(osg::Shader::FRAGMENT);
//	fragment_shader->loadShaderSourceFromFile( "../../shaders/lambert_fragment.glsl" );
	fragment_shader->loadShaderSourceFromFile( "C:/subversion/maya2osg/tests/shaders/lambert_fragment.glsl" );
	program->addShader( fragment_shader );
#else
	program->addShader( new osg::Shader(osg::Shader::FRAGMENT, LambertFragmentShaderSrc()) );
#endif

    state_set->setAttribute( program );

	// Uniform: LocalViewer
	state_set->addUniform( new osg::Uniform("LocalViewer", Config::instance()->getLocalViewer()) );

	// Uniform: NumEnabledLights
	state_set->addUniform( Lights::getUniformNumEnabledLights() );

	// Set texture samplers

	// we need to know what textures are connected to each channel, in what texture unit they are
	// and what texture coordinate set (UV set) they use.

	if( transparency ){
		state_set->setMode(GL_BLEND, osg::StateAttribute::ON);
		state_set->setAttribute( new osg::BlendFunc( Config::instance()->getBlendFuncSrc(),
											Config::instance()->getBlendFuncDst()) );
		state_set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	}
}
