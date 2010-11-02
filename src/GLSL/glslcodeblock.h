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
#ifndef _GLSLCODEBLOCK_H_
#define _GLSLCODEBLOCK_H_

#include "texturingconfig.h"

#include <osg/Referenced>
#include <osg/StateSet>
#include <string>
#include <maya/MObject.h>

/**
 *  GLSL Code Block
 *
 *  A GLSL code block is composed by three parts: 
 *  declarations, functions and inline code
 *
 *  @note Inherits from osg::Referenced to handle the
 *  allocation with smart pointers (osg::ref_ptr)
 */
class GLSLCodeBlock : public osg::Referenced {

public:

    /// Configure the CodeBlock for a channel of a surface shader (if anything connected)
    static GLSLCodeBlock *configure( const MObject &surface_shader, const std::string &channel,
                                     osg::StateSet &state_set, TexturingConfig &texturing_config );

    /// Build the code block for a Maya DG node of a shading network
    static GLSLCodeBlock *build( const MObject &node, osg::StateSet &sset, 
                                 TexturingConfig &texturing_config );

    /// Get the GLSL declarations (constants, global variables, uniforms, etc...)
    virtual std::string getDeclarations() = 0;

    /// Get the GLSL functions needed for this implementation
    virtual std::string getFunctions() = 0;

    /// Get the GLSL inline code
    virtual std::string getInlineCode() = 0;

};

#endif //_GLSLCODEBLOCK_H_
