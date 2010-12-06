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
#include <fstream>

// Dump shaders to disk (for debugging purposes)
//#define DUMP_SHADERS

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
    std::string fragment_shader_src = _surfaceShader->getFragmentShaderSrc();
    program->addShader( new osg::Shader(osg::Shader::FRAGMENT, fragment_shader_src) );
#  ifdef DUMP_SHADERS
    {
        MFnDependencyNode dn(_mayaShadingNetwork);
        std::string filename = dn.name().asChar();
        hygienizeName( filename );
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
    std::string vertex_shader_src = _surfaceShader->getVertexShaderSrc();
    program->addShader( new osg::Shader(osg::Shader::VERTEX, vertex_shader_src) );
#  ifdef DUMP_SHADERS
    {
        MFnDependencyNode dn(_mayaShadingNetwork);
        std::string filename = dn.name().asChar();
        hygienizeName( filename );
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
