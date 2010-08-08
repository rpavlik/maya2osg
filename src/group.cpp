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
#include "group.h"
#include "transform.h"
#include "cameraanimation.h"
#include "dagnode.h"

#include <maya/MFnDependencyNode.h>
#include <osg/Group>
#include <osg/LightSource>

#include <iostream>

osg::ref_ptr<osg::Node> Group::exporta(MDagPath &dp)
{
	osg::ref_ptr<osg::Group> osggroup;

	// Get the node of this path
	MObject node = dp.node();

	// 1. Create the adequate type of node
	if( node.hasFn(MFn::kTransform) ){
		osggroup = Transform::exporta(node);
	}
	else {
		// Generic group (kWorld)
		osggroup = new osg::Group();
	}

	// 2. Process and add children
	for(int i=0; i<dp.childCount(); i++){
		// Add child to the path and recursively call the exportation function
		MDagPath dpc(dp);
		dpc.push(dp.child(i));
		osg::ref_ptr<osg::Node> child = DAGNode::exporta(dpc);
		if(child.valid()){
			// ** Check ** If any children is a LightSource, deactivate culling 
			// for this group in order to apply the light
			// I don't really know why I did this here :-?
			if( dynamic_cast<osg::LightSource *>(child.get()) != NULL )
				osggroup->setCullingActive(false);
			osggroup->addChild(child.get());
		}
	}

	// 3. If there are no children, the node is ignored
	if( osggroup->getNumChildren() == 0 ){
		// Buuuuuuut, if there is an animation, it is saved to disk
		// because it can be a useful camera animation
		osg::AnimationPathCallback *cb = dynamic_cast< osg::AnimationPathCallback * >(osggroup->getUpdateCallback());
		if(cb){
			MFnDependencyNode dn(node);
			std::cout << "EXPORTING CAMERA ANIMATION: " << dn.name().asChar() << std::endl;
			CameraAnimation::save(cb->getAnimationPath(), std::string(dn.name().asChar()) + ".path" );
		}
		return NULL;
	}

    // Name the node (mesh)
	MFnDependencyNode dnodefn(node);
	osggroup->setName( dnodefn.name().asChar() );

	return (osg::Node *)osggroup.get();
}
