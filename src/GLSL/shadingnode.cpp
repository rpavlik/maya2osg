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
#include "shadingnode.h"
#include "shadingnetwork.h"
#include "shadingnodefactory.h"

#include "../shader.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

#include <sstream>


/// Constructor
ShadingNode::ShadingNode( const MObject &shading_node, ShadingNetwork &shading_network ) :
    _mayaShadingNode(shading_node),
    _shadingNetwork(shading_network)
{
}


/**
 *  Check if a variable was already computed (and so, its available to include it in GLSL code)
 */
bool ShadingNode::variableIsAvailable( const std::string &variable_name )
{
    return _computedVariables.find(variable_name) != _computedVariables.end();
}


/**
 *  Get the plug info for a channel of this Shading Node
 */
ShadingNode::Plug ShadingNode::getPlug(const std::string &channel)
{
    Plug plug;
    plug.channel = channel;
    plug.connectedShadingNode = NULL;
    if ( Shader::connectedChannel( _mayaShadingNode, channel) ){
        MObject node;
        Shader::getNodeConnectedToChannel( _mayaShadingNode, channel, node );
        plug.connectedShadingNode = ShadingNodeFactory::build( node, _shadingNetwork );
    }
    if ( plug.connectedShadingNode.valid() ){
        // Get the plug connected to plug
        MPlug remote_plug;
        Shader::getPlugConnectedToChannel( _mayaShadingNode, channel, remote_plug );
        std::string rp_name = remote_plug.partialName(false, false, false, false, false, true).asChar();
        plug.codeBlock = plug.connectedShadingNode->getCodeBlock( rp_name );
    }
    return plug;
}


/**
 *  Get the plug value, whether networked or not
 */
std::string ShadingNode::getPlugValue( Plug &plug )
{
    std::stringstream glsl_code;

    if ( plug.connectedShadingNode.valid() ) {
        return plug.codeBlock.accessCode;
    }
    else {
        MFnDependencyNode dn(_mayaShadingNode);
        MPlug maya_plug = dn.findPlug(plug.channel.c_str());
        if ( maya_plug.isCompound() ) {
            glsl_code << "vec" << maya_plug.numChildren() << "(";
            float val;
            maya_plug.child(0).getValue(val);
            glsl_code << val;
            for( int i=1 ; i < maya_plug.numChildren() ; i++ ) {
                maya_plug.child(i).getValue(val);
                glsl_code << "," << val;
            }
            glsl_code << ")";
        }
        else {
            float val;
            maya_plug.getValue( val );
            glsl_code << val;
        }
        return glsl_code.str();
    }
}
