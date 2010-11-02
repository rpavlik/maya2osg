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
#ifndef _GLSLCBFILETEXTURE_H_
#define _GLSLCBFILETEXTURE_H_

#include "glslcodeblock.h"

/**
 *
 */
class GLSLCBFileTexture : public GLSLCodeBlock {

public:
    /// Constructor
    GLSLCBFileTexture( const MObject &node, osg::StateSet& sset, 
                       TexturingConfig &texturing_config );

    /// Get the GLSL declarations (constants, global variables, uniforms, etc...)
    virtual std::string getDeclarations();

    /// Get the GLSL functions needed for this implementation
    virtual std::string getFunctions();

    /// Get the GLSL inline code
    virtual std::string getInlineCode();

private:

    /// Name of the texture sampler (GLSL uniform)
    std::string _samplerName;

    /// Number of texturing unit for the texture coordinates that should be used with this texture
    int _TCSetUnit;

};

#endif //_GLSLCBFILETEXTURE_H_
