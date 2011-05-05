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
#include "dagnode.h"
#include "group.h"
#include "mesh.h"
#include "pointlight.h"
#include "directionallight.h"
#include "spotlight.h"
#include "lights.h"
#include "camera.h"
#include "particle.h"
#include "nparticle.h"

#include <maya/MString.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <osg/Group>

#include <iostream>


/**
 *	Export Maya DAG node
 */
osg::ref_ptr<osg::Node> DAGNode::exporta(MDagPath &dp)
{
	// Get the node from the path
	MObject node = dp.node();
	MFnDependencyNode dnodefn(node);

    bool visible;
	dnodefn.findPlug("visibility").getValue(visible);
    if ( !visible ) {
        return NULL;
    }

	osg::ref_ptr<osg::Node> osgnode;

#ifdef _DEBUG
	std::cout << "Exporting node " << node.apiTypeStr() << " (" << 
		dnodefn.name().asChar() << ")" << std::endl;
#endif

    // Check what type of node it is

	// GROUPS
	if( node.hasFn( MFn::kWorld ) ||
		node.hasFn( MFn::kTransform ) ){
		return Group::exporta(dp);
	}
	// POLYGON MESHES (AND CUSTOM NODES)
	else if( node.hasFn( MFn::kMesh ) ){

		// Ignore Intermediate Objects ( Object Display ), mostly source of History and unwanted
		if( dnodefn.findPlug( "intermediateObject" ).asBool() )
			return NULL ;

		//return MeshOld::exporta(node);
		return Mesh::exporta(dp);

	}
	// NURBS
	else if( node.hasFn( MFn::kNurbsSurface ) ){
        // Geometry (NURBS)
		std::cout << "WARNING! This plug-in does not support NURBS by now, so you must convert "
			<< dp.fullPathName().asChar() << " to polygons" << std::endl;
        return NULL;
	}

	// CAMERAS
	else if( node.hasFn( MFn::kCamera ) ){
		return Camera::exporta(node);
	}

	// LIGTHS
	else if( node.hasFn( MFn::kAmbientLight ) ||
		node.hasFn( MFn::kAreaLight ) ||
		node.hasFn( MFn::kVolumeLight ) ) 
	{
		std::cout << "WARNING. The exporter does not support ambient, area or volume lights" << std::endl;
		return NULL;
	}
	else if( node.hasFn( MFn::kPointLight ) ) {
		return PointLight::exporta(node);
	}
	else if( node.hasFn( MFn::kDirectionalLight ) ) {
		return DirectionalLight::exporta(node);
	}
	else if( node.hasFn( MFn::kSpotLight ) ) {
		return SpotLight::exporta(node);
	}

	// PARTICLES
	else if( node.hasFn( MFn::kParticle ) ) {
		return Particle::exporta(node);
	}
	else if( node.hasFn( MFn::kNParticle ) ) {
		return NParticle::exporta(node);
	}

	// IGNORED TYPES OF NODE (groundplane, etc ...)

	else if(
		node.hasFn( MFn::kGroundPlane ) ||
		node.hasFn( MFn::kLocator )
	  ){
        return NULL;
	}

	else {
		std::cout << "ERROR. Unknown node type : " << dp.fullPathName().asChar() <<
			"(" << node.apiTypeStr() << "). Ignoring node." << std::endl;
        return NULL;
	}

    // Naming the node (group)
	osgnode->setName( dnodefn.name().asChar() );
    return osgnode;
}
