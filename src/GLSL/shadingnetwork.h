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
#ifndef _SHADINGNETWORK_H_
#define _SHADINGNETWORK_H_

#include <maya/MObject.h>
#include <osg/StateSet>
#include "texturingconfig.h"
#include "surfaceshader.h"
#include "shadingnode.h"

// Vertex attribute locations (see http://forum.openscenegraph.org/viewtopic.php?t=4653 and osgvertexattribute example)
#define TANGENT_ATTRIB_LOCATION 6
#define BINORMAL_ATTRIB_LOCATION 7

/**
 *  TO-DO: check what uniforms are used, to create 
 *  all the needed ones and only them
 *  *********** FIXME!!!
 */
class ShadingNetwork {

public:

    /// Constructor
    ShadingNetwork( const MObject &shading_network, TexturingConfig &texturing_config, 
                    osg::StateSet &state_set );

    /// Register a Shading Node
    void registerShadingNode( ShadingNode *sn, const std::string &name );

    /// Get a registered Shading Node by name
    osg::ref_ptr<ShadingNode> getShadingNode( const std::string &name );

    inline TexturingConfig &getTexturingConfig()
    { return _texturingConfig; }

    inline osg::StateSet &getStateSet()
    { return _stateSet; }

private:

    /// Shading Network Maya Object
    const MObject &_mayaShadingNetwork;

    /// Texturing configuration
    TexturingConfig &_texturingConfig;

    /// StateSet
    osg::StateSet &_stateSet;

    /// Surface Shader root of the Shading Network
    osg::ref_ptr<SurfaceShader> _surfaceShader;

    /// Map of Shading Nodes created in the Shading Network
    std::map<std::string, osg::ref_ptr<ShadingNode> > _shadingNodesMap;

};

#endif //_SHADINGNETWORK_H_
