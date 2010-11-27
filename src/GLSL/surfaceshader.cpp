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
#include "surfaceshader.h"
#include "shadingnetwork.h"

#include "../config.h"
#include "../shader.h"

#include <maya/MFnDependencyNode.h>


/**
 *
 */
SurfaceShader::SurfaceShader( const MObject &shading_node, ShadingNetwork &shading_network ) :
    ShadingNode(shading_node, shading_network )
{
}


/**
 * Check whether this shader uses transparency
 */
bool SurfaceShader::hasTransparency()
{
    MFnDependencyNode dn(_mayaShadingNode);
    return Shader::connectedChannel(_mayaShadingNode, "transparency")
		|| dn.findPlug("transparencyR").asFloat() > 0.0
		|| dn.findPlug("transparencyG").asFloat() > 0.0
		|| dn.findPlug("transparencyB").asFloat() > 0.0;
}


/**
 *  Check whether this shader has a bump map connected
 */
bool SurfaceShader::hasBumpMap()
{
	if ( Config::instance()->getEnableBumpMapping() )
	    return Shader::connectedChannel(_mayaShadingNode, "normalCamera");
	else
		return false;
}
