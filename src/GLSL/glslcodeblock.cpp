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
#include "glslcodeblock.h"
#include "glslcbfiletexture.h"
#include "texturingconfig.h"
#include "../shader.h"

#include <iostream>

/**
 *  Build the code block for a Maya DG node of a shading network
 */
GLSLCodeBlock *GLSLCodeBlock::build( const MObject &node, osg::StateSet& sset, 
                                     TexturingConfig &texturing_config )
{
    if ( node.hasFn( MFn::kFileTexture ) ) {
        return new GLSLCBFileTexture(node, sset, texturing_config);
    }
    else if ( node.hasFn( MFn::kBump ) ) {
//        return GLSLCBBump();
    }
    // THE REST OF NODES...

    // Unsupported node
    std::cout << "GLSLCodeBlock::build - ERROR : Unsupported node in the shading network (" 
        << node.apiTypeStr() << ")" << std::endl;
    // *****
    // TO-DO: throw an exception to abort the shader construction
    //  or return a default value (i.e. 0) and "show must go on"...
    return NULL;
}


/**
 *  Configure the CodeBlock for a channel of a surface shader (if anything connected)
 */
GLSLCodeBlock *GLSLCodeBlock::configure( const MObject &surface_shader, const std::string &channel,
                                         osg::StateSet &state_set, TexturingConfig &texturing_config )
{
    if ( Shader::connectedChannel( surface_shader, channel ) ) {
        MObject node;
        Shader::getNodeConnectedToChannel( surface_shader, channel, node );
        return GLSLCodeBlock::build( node, state_set, texturing_config );
    }
    return NULL;
}
