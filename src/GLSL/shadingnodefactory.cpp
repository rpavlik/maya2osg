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
#include "shadingnodefactory.h"
#include "shadingnetwork.h"

#include <maya/MFnDependencyNode.h>

// This should use clonation and autoregistering of shading nodes to the factory
#include "lambert.h"
#include "phong.h"
#include "blinn.h"
#include "flatshader.h"

#include "filetexture.h"
#include "bump2d.h"
#include "../config.h"

/**
 *  Build a shading node
 */
ShadingNode *ShadingNodeFactory::build( const MObject &node, ShadingNetwork &shading_network )
{
    MFnDependencyNode dn(node);
    std::string node_name = dn.name().asChar();

    osg::ref_ptr<ShadingNode> sn = shading_network.getShadingNode(node_name);

    if ( sn.valid() ) {
        return sn.get();
    }

    if ( node.hasFn( MFn::kPhong ) ) {
        sn = new Phong( node, shading_network );
    }
    else if ( node.hasFn( MFn::kBlinn ) ) {
        sn = new Blinn( node, shading_network );
    }
    else if ( node.hasFn( MFn::kLambert ) ) {
        sn = new Lambert( node, shading_network );
    }
    else if ( node.hasFn( MFn::kSurfaceShader ) ) {
        sn = new FlatShader( node, shading_network );
    }
    else if ( node.hasFn( MFn::kFileTexture ) ) {
        sn = new FileTexture( node, shading_network );
    }
    else if ( node.hasFn( MFn::kBump ) ) {
		if ( Config::instance()->getEnableBumpMapping() ) {
	        sn = new Bump2D( node, shading_network );
		}
		else {
			std::cerr << "Bump mapping disabled in the exporter configuration" << std::endl;
		}
    }
    // *** ... TO-DO: CHECK FOR OTHER SHADING NODES...
    else {
        std::cerr << "ERROR: Unsupported shading node : " << node.apiTypeStr() << std::endl;
    }

    if ( sn.valid() ) {
        shading_network.registerShadingNode( sn.get(), node_name );
    }

    return sn.get();
}
