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
#ifndef _SURFACESHADER_H_
#define _SURFACESHADER_H_

#include "shadingnode.h"

#include <maya/MObject.h>

/**
 *  Surface Shader is an specialization of Shading Node
 *  that can feed a fragment shader
 *
 *  @note Every Surface Shader must offer the "fragmentOutput"
 *  special plug, not present in Maya, to use it in the fragment
 *  shader without having to access separately color and alpha
 *
 *  @note The root of a Shading Network is always a Surface Shader
 */
class SurfaceShader : public ShadingNode {

public:

    /// Constructor
    SurfaceShader( const MObject &node, ShadingNetwork &shading_network );

    /// Check whether this shader uses transparency
    virtual bool hasTransparency();

    /// Check whether this shader has a bump map connected
    virtual bool hasBumpMap();

};

#endif //_SURFACESHADER_H_
