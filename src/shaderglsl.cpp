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


const std::string fragment_src_lambert =
"\n"
"uniform vec3 color;\n"
"\n"
"void main() {\n"
"	gl_FragColor = vec4(color, 1.0);\n"
"}\n"
;


/**
 *	Configure the shaders for the StateSet of the ShadingEngine/ShadingGroup object
 *
 *	@todo Recheck the need and use of parameter "textures"  ***** FIXME!!!
 */
void ShaderGLSL::exporta(const MObject &shading_engine, const MObjectArray &textures, osg::ref_ptr<osg::StateSet> state_set)
{
	MObject surface_shader;
	Shader::getSurfaceShader( shading_engine, surface_shader );

	if ( surface_shader.hasFn(MFn::kLambert) ) {
		exportLambert( surface_shader, textures, state_set );
	}
	//else if ...
	// TO-DO : rest (non Lambert-derived) of Maya shaders *****  FIXME!!!
}


/**
 *	Create the stateset for a Lambert material
 *
 *	********* UNDER CONSTRUCTION!!!   --- FIXME!!!!!!!!
 */
void ShaderGLSL::exportLambert(const MObject &surface_shader, const MObjectArray &textures, osg::ref_ptr<osg::StateSet> state_set)
{
	MFnDependencyNode dn(surface_shader);

	// Check if there is transparency
	bool transparency = Shader::connectedTexture(surface_shader, "transparency") 
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
	// FIXME!!! *** num_tc_sets
	int num_tc_sets = 1;
	program->addShader( new osg::Shader(osg::Shader::VERTEX, LambertVertexShaderSrc(num_tc_sets)) );
#endif
	// Fragment shader
#if 1
	osg::Shader *fragment_shader = new osg::Shader(osg::Shader::FRAGMENT);
//	fragment_shader->loadShaderSourceFromFile( "../../shaders/lambert_fragment.glsl" );
	fragment_shader->loadShaderSourceFromFile( "C:/subversion/maya2osg/tests/shaders/lambert_fragment_notex.glsl" );
	program->addShader( fragment_shader );
#else
	program->addShader( new osg::Shader(osg::Shader::FRAGMENT, fragment_src_lambert) );
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
