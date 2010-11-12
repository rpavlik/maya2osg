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
#ifndef _SHADINGNODE_H_
#define _SHADINGNODE_H_

#include "texturingconfig.h"

#include <osg/Referenced>
#include <osg/ref_ptr>
#include <string>
#include <maya/MObject.h>

class ShadingNetwork;

/**
 *  This class represents a shading node in a Maya shading network
 *  and provide the methods to build the GLSL source code to 
 *  implement the Maya shading node behaviour.
 */
class ShadingNode : public osg::Referenced {

public:

    typedef struct {
        std::string declarations;   ///< Declarations
        std::string functions;      ///< Functions
        std::string computeCode;    ///< Inserted code for computing the output value
        std::string accessCode;     ///< Code to access the computed value
    } CodeBlock;

    /// Constructor
    ShadingNode( const MObject &shading_node, ShadingNetwork &shading_network );

    /// Get the GLSL Codeblock for a plug
    virtual CodeBlock getCodeBlock( const std::string &plug_name ) = 0;

protected:

    const MObject& _mayaShadingNode;

    ShadingNetwork& _shadingNetwork;

    /// Set of the GLSL variables already constructed (to avoid repeating code)
    std::set<std::string> _computedVariables; 

    /// Check if a variable was already computed (and so, its available to include it in GLSL code)
    bool variableIsAvailable( const std::string &variable_name );

    typedef struct {
        osg::ref_ptr<ShadingNode> connectedShadingNode;
        CodeBlock codeBlock;
        std::string channel;
    } Plug;

    /// Get the plug info for a channel of this Shading Node
    Plug getPlug(const std::string &channel);

    /// Get the plug value, whether networked or not
    std::string getPlugValue( Plug &plug );

};

#endif //_SHADINGNODE_H_
