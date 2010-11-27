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
#include "shadingnetwork.h"
#include "surfaceshader.h"
#include "shadingnodefactory.h"
#include "../shader.h"
#include "../config.h"
#include "../lights.h"
#include "../common.h"

#include <maya/MFnDependencyNode.h>
#include <sstream>
#include <fstream>

// Dump shaders to disk (for debugging purposes)
#define DUMP_SHADERS

/**
 *  Constructor
 */
ShadingNetwork::ShadingNetwork( const MObject &shading_network, TexturingConfig &texturing_config,
                                osg::StateSet &state_set ) :
    _mayaShadingNetwork(shading_network),
    _texturingConfig(texturing_config),
    _stateSet(state_set)
{
    // Build the shading network that will be used to 
    // create the GLSL program.
    // The first node of the shading network will be a
    // surface material, that has three output plugs
    // that could interest us: 
    //
    //      outColor
    //      outTransparency
    //      outGlowColor

    // Set the OpenGL material (will be accessed by the shader) **** FIXME!!!
    bool transparent;
	_stateSet.setAttribute( Shader::material(_mayaShadingNetwork, transparent).get() );

    // Build the Surface Shader (root of the Shading Network)
    _surfaceShader = dynamic_cast<SurfaceShader *>(ShadingNodeFactory::build( _mayaShadingNetwork, *this ));

	// GLSL Program
	osg::Program *program = new osg::Program();

    if ( _surfaceShader->hasBumpMap() ) {
        program->addBindAttribLocation( "inTangent", TANGENT_ATTRIB_LOCATION );
    }

    // NOTE: We build FIRST the fragment shader to know what textures are used
    // and then in the vertex shader generate the texture coordinates for them

    // Fragment shader
#if 0
	osg::Shader *fragment_shader = new osg::Shader(osg::Shader::FRAGMENT);
    {
        MFnDependencyNode dn(_mayaShadingNetwork);
        std::string filename = dn.name().asChar();
        filename += "_fragment.glsl";
	    fragment_shader->loadShaderSourceFromFile( filename );
    	program->addShader( fragment_shader );
    }
#else
    std::string fragment_shader_src = getFragmentShaderSrc();
    program->addShader( new osg::Shader(osg::Shader::FRAGMENT, fragment_shader_src) );
#  ifdef DUMP_SHADERS
    {
        MFnDependencyNode dn(_mayaShadingNetwork);
        std::string filename = dn.name().asChar();
        filename += "_fragment.glsl";
        std::ofstream fout(filename.c_str());
        fout << fragment_shader_src;
        fout.close();
    }
#  endif
#endif

    // Vertex shader
#if 0
	osg::Shader *vertex_shader = new osg::Shader(osg::Shader::VERTEX);
    {
        MFnDependencyNode dn(_mayaShadingNetwork);
        std::string filename = dn.name().asChar();
        filename += "_vertex.glsl";
    	vertex_shader->loadShaderSourceFromFile( filename );
    	program->addShader( vertex_shader );
    }
#else
    std::string vertex_shader_src = getVertexShaderSrc();
    program->addShader( new osg::Shader(osg::Shader::VERTEX, vertex_shader_src) );
#  ifdef DUMP_SHADERS
    {
        MFnDependencyNode dn(_mayaShadingNetwork);
        std::string filename = dn.name().asChar();
        filename += "_vertex.glsl";
        std::ofstream fout(filename.c_str());
        fout << vertex_shader_src;
        fout.close();
    }
#  endif
#endif

    _stateSet.setAttribute( program );

    // THESE UNIFORMS SHOULD BE PROBABLY REMOVED !!!   FIXME!!! *****

	// Uniform: LocalViewer
	_stateSet.addUniform( new osg::Uniform("LocalViewer", Config::instance()->getLocalViewer()) );

	// Uniform: NumEnabledLights
	_stateSet.addUniform( Lights::getUniformNumEnabledLights() );

	// Check if there is transparency to activate the blending in the StateSet
	if( _surfaceShader->hasTransparency() ){
		_stateSet.setMode(GL_BLEND, osg::StateAttribute::ON);
		_stateSet.setAttribute( new osg::BlendFunc( Config::instance()->getBlendFuncSrc(),
											Config::instance()->getBlendFuncDst()) );
		_stateSet.setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	}
}


/**
 *	Build the vertex shader GLSL source code for generic materials
 */
std::string ShadingNetwork::getVertexShaderSrc()
{
    bool bump_map = _surfaceShader->hasBumpMap();

    std::string shader_src =
"// Exported with Maya2OSG " VERSION "\n"
"// http://maya2osg.sourceforge.net\n"
"#version 120\n"
"\n"
"varying vec3 ecPosition3;\n"
"varying vec3 normal;\n";

    if ( bump_map ) {
        shader_src += 
"attribute vec3 inTangent;\n"
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
"	// Normals (in eye coordinates, we will normalize them after interpolation)\n"
"	normal = normalize(gl_NormalMatrix * gl_Normal);\n";

    if ( bump_map ) {
        shader_src +=
"    normal = normalize(normal);\n"
"    tangent = normalize(gl_NormalMatrix * inTangent);\n"
"    binormal = cross(normal, tangent);\n"
;
    }

    shader_src +=
"\n"
;
    for ( std::set<int>::const_iterator i = _texturingConfig.getTCSetsUnitsUsed().begin() ; 
            i != _texturingConfig.getTCSetsUnitsUsed().end() ; i++ ) 
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
 *  Build the fragment shader GLSL source code for this Shading Network
 */
std::string ShadingNetwork::getFragmentShaderSrc()
{
    SurfaceShader::CodeBlock cb_fragment_output = _surfaceShader->getCodeBlock("fragmentOutput");
    bool bump_map = _surfaceShader->hasBumpMap();

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


/**
 *  Register a Shading Node
 */
void ShadingNetwork::registerShadingNode( ShadingNode *sn, const std::string &name )
{
    _shadingNodesMap[name] = sn;
}


/**
 *  Get a registered Shading Node by name
 */
osg::ref_ptr<ShadingNode> ShadingNetwork::getShadingNode( const std::string &name )
{
    if ( _shadingNodesMap.find(name) != _shadingNodesMap.end() ) {
        return _shadingNodesMap[name];
    }
    return NULL;
}
