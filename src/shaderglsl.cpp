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

#include <osg/BlendFunc>

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>


////////////////////////////////////////////////////////////////////////////////

// NOTE: See Orange Book chapter 9 (Emulating OpenGL Fixed Functionality)

const std::string vertex_src_lambert =
"\n"
"uniform vec3 color;\n"
"\n"
"void main() {\n"
"	// Transform vertex from object space to clip space\n"
"	gl_Position = ftransform();\n"
"\n"
"	// NOTE: Lighting computations are performed in eye coordinates\n"
"	// because OpenGL specifies that light positions are transformed\n"
"	// by the modelview matrix when they are provided to OpenGL\n"
"\n"
"//	ecPosition = gl_ModelViewMatrix * gl_Vertex;\n"
"//	ecPosition3 = (vec3(ecPosition)) / ecPosition.w;\n"
"\n"
"//	normal = gl_NormalMatrix * gl_Normal;\n"
"//	normal = normalize(normal);\n"
"\n"
"	// For every used texture unit... ***\n"
"//	gl_TexCoord[0] = gl_TextureMatrix[0] * glMultiTexCoord0;\n"
"\n"
"}\n"
;

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
		exportLambert( surface_shader, state_set );
	}
	//else if ...
	// TO-DO : rest (non Lambert-derived) of Maya shaders *****  FIXME!!!
}


/**
 *	Create the stateset for a Lambert material
 *
 *	********* UNDER CONSTRUCTION!!!   --- FIXME!!!!!!!!
 */
void ShaderGLSL::exportLambert(const MObject &surface_shader, osg::ref_ptr<osg::StateSet> state_set)
{
	MFnDependencyNode dn(surface_shader);

	// **** FIXME!!! check if there is transparency
	bool transparency = false;

	// Export common material attributes
	// FIXME *** -> this code will be common to every shader that inherit from lambert node,
	//		it will be put on a separate function/method

	// Color
	// Transparency
	// Ambient Color
	// Incandescence
	// Bump Mapping
	// Diffuse
	// Translucence (*)
	// Translucence Depth (*)
	// Translucence Focus (*)

	// (*) Parameter currently ignored

	// GLSL Program
	osg::Program *program = new osg::Program();
#if 1
	osg::Shader *vertex_shader = new osg::Shader(osg::Shader::VERTEX);
	osg::Shader *fragment_shader = new osg::Shader(osg::Shader::FRAGMENT);
	vertex_shader->loadShaderSourceFromFile( "../../shaders/lambert_vertex.glsl" );
	fragment_shader->loadShaderSourceFromFile( "../../shaders/lambert_fragment.glsl" );
	program->addShader( vertex_shader );
	program->addShader( fragment_shader );
#else
	program->addShader( new osg::Shader(osg::Shader::VERTEX, vertex_src_lambert) );
	program->addShader( new osg::Shader(osg::Shader::FRAGMENT, fragment_src_lambert) );
#endif
	state_set->setAttribute( program );

	// WARNING - Code under development. By now we don't consider textures 
	// connected to this attribue, just a constant value
	osg::Vec3 color( dn.findPlug("colorR").asFloat(),
					dn.findPlug("colorG").asFloat(),
					dn.findPlug("colorB").asFloat() );
	state_set->addUniform( new osg::Uniform("color", color) );

	if( transparency ){
		state_set->setMode(GL_BLEND, osg::StateAttribute::ON);
		state_set->setAttribute( new osg::BlendFunc( Config::instance()->getBlendFuncSrc(),
											Config::instance()->getBlendFuncDst()) );
		state_set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	}
}
