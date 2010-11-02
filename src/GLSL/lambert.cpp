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
#include "glsllighting.h"
#include "glslcodeblock.h"
#include "../shader.h"
#include "../config.h"
#include "../lights.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

#include <osg/CullFace>

#include <fstream>
#include <sstream>

// Dump shaders to disk (for debugging purposes)
#define DUMP_SHADERS

/**
 *	Build the vertex shader GLSL source code for Lambert materials
 *
 *	@param num_tc_sets	Number of texture coordinate sets used by this shader
 */
std::string Lambert::getVertexShaderSrc( TexturingConfig & texturing_config )
{
	std::string shader_src =
"#version 120\n"
"\n"
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
    for ( std::set<int>::const_iterator i = texturing_config.getTCSetsUnitsUsed().begin() ; 
            i != texturing_config.getTCSetsUnitsUsed().end() ; i++ ) 
    {
		std::stringstream tc_set;
		tc_set << *i;
		shader_src += 
"	gl_TexCoord[" + tc_set.str() + "] = gl_TextureMatrix[" + 
				tc_set.str() + "] * gl_MultiTexCoord" + tc_set.str() + ";\n"
"}\n"
;
	}
#ifdef DUMP_SHADERS
    std::ofstream fout("lambert_vertex.glsl");
    fout << shader_src;
    fout.close();
#endif
	return shader_src;
}


/**
 *  Build the fragment shader GLSL source code for Lambert materials
 */
std::string Lambert::getFragmentShaderSrc( osg::StateSet &state_set,
                                           TexturingConfig &texturing_config,
                                           GLSLCodeBlock *color_cb,
                                           GLSLCodeBlock *ambient_cb,
                                           GLSLCodeBlock *emissive_cb )
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
"\n";

    // GLSL Code Blocks declarations for each channel
    // (include uniforms for texture samplers)

    // Channel: Diffuse color
    if ( color_cb ) {
        shader_src += 
"uniform float diffuseCoeff;\n" +
            color_cb->getDeclarations() +
            "\n" + color_cb->getFunctions() + "\n";
    }
    // Channel: Ambient color
    if ( ambient_cb ) {
        shader_src += ambient_cb->getDeclarations() +
            "\n" + ambient_cb->getFunctions() + "\n";
    }
    // Channel: Emissive color
    if ( emissive_cb ) {
        shader_src += emissive_cb->getDeclarations() +
            "\n" + emissive_cb->getFunctions() + "\n";
    }

    shader_src += 
GLSLLighting::getDirectionalLightFunctionWithoutSpecular() +
"\n" +
GLSLLighting::getPointLightFunctionWithoutSpecular() +
"\n" +
GLSLLighting::getSpotLightFunctionWithoutSpecular() +
"\n"
"\n"
"void main() {\n"
"\n"
"    vec3 nnormal;\n";

	// TO-DO : **** FIXME!!!
	// Adjust normal by bump map
	// ...

    bool opposite = false;
    osg::StateAttribute *sa = state_set.getAttribute( osg::StateAttribute::CULLFACE );
    if ( sa ) {
        osg::CullFace *cf = dynamic_cast<osg::CullFace*>(sa);
        if ( cf ) {
            // If culling front facing polygons, we consider it the opposite direction
            opposite = cf->getMode() == osg::CullFace::FRONT;
        }
    }

    // Check if the StateSet has backface culling enabled 
    // and add code to the shader to perform it 
    if ( state_set.getMode(GL_CULL_FACE) == osg::StateAttribute::ON ) {
        if ( opposite ) {
            shader_src += 
"    if ( gl_FrontFacing )\n"
"        discard;\n"
"    else\n"
"        nnormal = normalize(-normal);\n";
        }
        else {
            shader_src += 
"    if ( !gl_FrontFacing )\n"
"        discard;\n"
"    else\n"
"        nnormal = normalize(normal);\n";
        }
        shader_src += 
"\n";
    }
    else {
        if ( opposite ) {
            shader_src += 
"    if ( !gl_FrontFacing )\n";
        }
        else {
            shader_src += 
"    if ( gl_FrontFacing )\n";
        }
        shader_src += 
"        nnormal = normalize(normal);\n"
"    else\n"
"        nnormal = normalize(-normal);\n"
"\n";
    }

    shader_src +=
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
"	        DirectionalLight(i, nnormal, amb, diff);\n"
"	    else if (gl_LightSource[i].spotCutoff == 180.0)\n"
"	        PointLight(i, eye, ecPosition3, nnormal, amb, diff);\n"
"	    else\n"
"	        SpotLight(i, eye, ecPosition3, nnormal, amb, diff);\n"
"	}\n"
"\n"
#if 0
"	// NOTE: gl_FrontLightModelProduct.sceneColor = gl_FrontMaterial.emission + gl_FrontMaterial.ambient * gl_LightModel.ambient\n"
"	vec4 color = gl_FrontLightModelProduct.sceneColor +\n"
"	            amb * gl_FrontMaterial.ambient +\n"
"	            diff * gl_FrontMaterial.diffuse;\n"
#else
"\n";

    // Channel : Diffuse color
    if ( color_cb ) {
        shader_src +=
"    vec4 diffuse_color = " + color_cb->getInlineCode() + " * diffuseCoeff;\n";
    }
    else {
        shader_src +=
"    vec4 diffuse_color = gl_FrontMaterial.diffuse;\n";
    }

    // Channel : Ambient color
    if ( ambient_cb ) {
        shader_src +=
"    vec4 ambient_color = " + ambient_cb->getInlineCode() + ";\n";
    }
    else {
        shader_src +=
"    vec4 ambient_color = gl_FrontMaterial.ambient;\n";
    }

    // Channel : Emissive color
    if ( emissive_cb ) {
        shader_src +=
"    vec4 emissive_color = " + emissive_cb->getInlineCode() + ";\n";
    }
    else {
        shader_src +=
"    vec4 emissive_color = gl_FrontMaterial.emission;\n";
    }

    shader_src +=
"\n"
"	vec4 color = emissive_color +\n"
"	            (amb + gl_LightModel.ambient) * ambient_color +\n"
"	            diff * diffuse_color;\n";
#endif

    shader_src +=
"\n"
"	gl_FragColor = color;\n"
"}\n";
#ifdef DUMP_SHADERS
    std::ofstream fout("lambert_fragment.glsl");
    fout << shader_src;
    fout.close();
#endif
    return shader_src;
}


/**
 *	Configure the StateSet with a GLSL program implementing the Lambert shader
 */
void Lambert::configureGLSLProgram(const MObject &surface_shader,
                                   TexturingConfig &texturing_config,
                                   osg::StateSet &state_set)
{
	MFnDependencyNode dn(surface_shader);
    bool transparent;

    // Set the OpenGL material (will be accessed by the shader)
	state_set.setAttribute( Shader::material(surface_shader, transparent).get() );

    // Check connections to supported channels and build GLSL Code Blocks needed to build the Fragment Shader

    // Channel: color (diffuse)
    osg::ref_ptr<GLSLCodeBlock> color_cb = GLSLCodeBlock::configure(surface_shader, "color", state_set, texturing_config);
    // opacity (only alpha channel of color)
    // ambient color
    osg::ref_ptr<GLSLCodeBlock> ambient_cb = GLSLCodeBlock::configure(surface_shader, "ambientColor", state_set, texturing_config);
    // incandescence (emissive)
    osg::ref_ptr<GLSLCodeBlock> emissive_cb = GLSLCodeBlock::configure(surface_shader, "incandescence", state_set, texturing_config);
    // bump map
    // *** TO-DO ...

	// GLSL Program
	osg::Program *program = new osg::Program();

    // Vertex shader
#if 0
	osg::Shader *vertex_shader = new osg::Shader(osg::Shader::VERTEX);
	vertex_shader->loadShaderSourceFromFile( "lambert_vertex.glsl" );
	program->addShader( vertex_shader );
#else
    program->addShader( new osg::Shader(osg::Shader::VERTEX, getVertexShaderSrc(texturing_config)) );
#endif

    // Fragment shader
#if 0
	osg::Shader *fragment_shader = new osg::Shader(osg::Shader::FRAGMENT);
	fragment_shader->loadShaderSourceFromFile( "lambert_fragment.glsl" );
	program->addShader( fragment_shader );
#else
    program->addShader( new osg::Shader(osg::Shader::FRAGMENT, 
                        getFragmentShaderSrc( state_set, texturing_config,
                                              color_cb.get(),
                                              ambient_cb.get(),
                                              emissive_cb.get()
                                              )));
#endif

    state_set.setAttribute( program );

    // If there is a network connected to color, its value must be multiplied 
    // with the diffuse coefficient. If there is not, it will be already multplied
    // in the exported Material (see shader.cpp)
    if ( color_cb.valid() ) {
        float dc;
        dn.findPlug("diffuse").getValue(dc);
        state_set.addUniform( new osg::Uniform("diffuseCoeff", dc) );
    }

	// Uniform: LocalViewer
	state_set.addUniform( new osg::Uniform("LocalViewer", Config::instance()->getLocalViewer()) );

	// Uniform: NumEnabledLights
	state_set.addUniform( Lights::getUniformNumEnabledLights() );

    // Uniform: numTexCoordSets
    state_set.addUniform( new osg::Uniform("numTexCoordSets", texturing_config.getTCSetsUnitsUsed().size()) );

	// Check if there is transparency to activate the blending in the StateSet
	transparent = Shader::connectedChannel(surface_shader, "transparency")
		|| dn.findPlug("transparencyR").asFloat() > 0.0
		|| dn.findPlug("transparencyG").asFloat() > 0.0
		|| dn.findPlug("transparencyB").asFloat() > 0.0;
	if( transparent ){
		state_set.setMode(GL_BLEND, osg::StateAttribute::ON);
		state_set.setAttribute( new osg::BlendFunc( Config::instance()->getBlendFuncSrc(),
											Config::instance()->getBlendFuncDst()) );
		state_set.setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	}
}
