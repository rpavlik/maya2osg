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
#ifndef _FLATSHADER_H_
#define _FLATSHADER_H_

#include "surfaceshader.h"

#include <string>


/**
 *  The flat shader just define an output color.
 *  It ignores all the lights in the scene
 *  It corresponds to the Maya "Surface Shader"
 */
class FlatShader : public SurfaceShader {

public:

    /// Constructor
    FlatShader( const MObject &shading_node, ShadingNetwork &shading_network );

    /// Get the GLSL Codeblock for a plug
    virtual CodeBlock getCodeBlock( const std::string &plug_name );

    /// Flat shader has no bump map
    virtual bool hasBumpMap()
    { return false; }

    /// Get the vertex shader GLSL source code
    virtual std::string getVertexShaderSrc();

    /// Get the fragment shader GLSL source code
    virtual std::string getFragmentShaderSrc();

};

#endif //_FLATSHADER_H_
