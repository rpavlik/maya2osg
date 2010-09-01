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
#ifndef _IMPORTERVISITOR_H_
#define _IMPORTERVISITOR_H_

#include <maya/MGlobal.h>

#include <osg/NodeVisitor>
#include <osg/Geometry>
#include <osg/ShapeDrawable>

class ImporterVisitor : public osg::NodeVisitor {

private:

	/// Parent node to which created nodes will be added
	MObject _parent;

public:

	/// Constructor
	ImporterVisitor() : NodeVisitor(TRAVERSE_ALL_CHILDREN)
	{ _parent = MObject::kNullObj; }


	virtual void apply (osg::Group &node);
	virtual void apply (osg::Geode &node);
	virtual void apply (osg::Transform &node);

	void importDrawable(osg::Drawable *drawable);
	void importGeometry(osg::Geometry *geometry, MObject &shading_engine);
	void importShapeDrawable(osg::ShapeDrawable *shape_drawable, MObject &shading_engine);

	/// Create the Maya Shading Node from OSG StateSet
	void createShadingEngine( osg::StateSet *ss, MObject &shading_engine );

};


#endif // _IMPORTERVISITOR_H_
