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
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "group.h"
#include <osg/AnimationPath>
#include <maya/MFnAnimCurve.h>
#include <maya/MFnMotionPath.h>

class Transform {

public:
	/// Export a Transform node (including callbacks for related animations)
	static osg::ref_ptr<osg::Group> exporta(MObject &obj);

private:
	/// Check if this node has animations connected (AnimCurves or MotionPaths)
	static bool hasAnimation(MObject &obj, MFn::Type &type);

	/// Build the animation from Maya AnimCurves
	static osg::ref_ptr<osg::AnimationPath> animCurve2AnimationPath(MObject &obj);
	/// Build the animation from Maya MotionPath
	static osg::ref_ptr<osg::AnimationPath> motionPath2AnimationPath(MObject &obj);
	/// Build the animation from any Maya animated Transform
	static osg::ref_ptr<osg::AnimationPath> animatedTransform2AnimationPath(MObject &obj);

	/// Get the position of a ControlPoint
	static osg::Vec3 getCPPosition(const MObject &obj);
	/// Get the rotation of a ControlPoint
	static osg::Quat getCPRotation(const MObject &obj);
	/// Get the scale of a ControlPoint
	static osg::Vec3 getCPScale(const MObject &obj);

};

#endif //_TRANSFORM_H_
