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
#include "transform.h"
#include "config.h"

#include <osg/MatrixTransform>
#include <osg/Matrix>
#include <osg/Quat>

#include <maya/MFnTransform.h>
#include <maya/MMatrix.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MEulerRotation.h>
#include <maya/MAnimControl.h>
#include <maya/MQuaternion.h>

#include <iostream>

// New strategy to export the animations. We keep it removable while testing and debugging
#define GENERIC_EXPORTER

/**
 *	Check if this node has animations connected (AnimCurves or MotionPaths)
 *	and indicate the type of the animation found (if any)
 */
bool Transform::hasAnimation(MObject &obj, MFn::Type &type)
{
	MFnDependencyNode dn(obj);
	MPlugArray conns;
	dn.getConnections(conns);
	for(int i=0; i<conns.length(); i++){
		MPlug conn = conns[i];
		if( conn.name() == dn.name() + ".translateX" ||
			conn.name() == dn.name() + ".translateY" ||
			conn.name() == dn.name() + ".translateZ" ||
			conn.name() == dn.name() + ".rotateX" ||
			conn.name() == dn.name() + ".rotateY" ||
			conn.name() == dn.name() + ".rotateZ" ||
			conn.name() == dn.name() + ".scaleX" ||
			conn.name() == dn.name() + ".scaleY" ||
			conn.name() == dn.name() + ".scaleZ" ){

			MPlugArray connectedTo;
			// Get all connections having this node as destination
			conn.connectedTo(connectedTo, true, false);
#ifdef GENERIC_EXPORTER
			if ( connectedTo.length() > 0 ) {
				return true;
			}
#else
			for(int j=0; j<connectedTo.length(); j++){
				MPlug origin = connectedTo[j];
				MObject origin_node = origin.node();
				if(origin_node.hasFn(MFn::kAnimCurve)){
					type = MFn::kAnimCurve;
					return true;
				}
				else if(origin_node.hasFn(MFn::kMotionPath)){
					type = MFn::kMotionPath;
					return true;
				}
			}
#endif
		}
	}
	type = MFn::kInvalid;
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *	Get the position of a ControlPoint
 */
osg::Vec3 Transform::getCPPosition(const MObject &obj)
{
	MFnTransform tr(obj);

	MVector p = tr.translation(MSpace::kTransform);
	return osg::Vec3(p.x, p.y, p.z);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *	Get the rotation of a ControlPoint
 */
osg::Quat Transform::getCPRotation(const MObject &obj)
{
	MFnTransform tr(obj);

	// Get the rotation as a quaternion
	double qx, qy, qz, qw;
	tr.getRotationQuaternion(qx, qy, qz, qw, MSpace::kTransform);
	return osg::Quat(qx, qy, qz, qw);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *	Get the scale of a ControlPoint
 */
osg::Vec3 Transform::getCPScale(const MObject &obj)
{
	MFnTransform tr(obj);
	double scale[3];

	tr.getScale(scale);

	return osg::Vec3(scale[0], scale[1], scale[2]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 *	Build the animation from a Maya generic animated Transform
 */
osg::ref_ptr<osg::AnimationPath> Transform::animatedTransform2AnimationPath(MObject &obj)
{
	osg::ref_ptr<osg::AnimationPath> anim = new osg::AnimationPath();

	MTime start = MAnimControl::animationStartTime();
	MTime end = MAnimControl::animationEndTime();
	for( MTime t = start ; t <= end ; t += Config::instance()->getAnimSampleBy() ) {
		// Set the right time in Maya timeline so all properties are updated
		MAnimControl::setCurrentTime(t);

		anim->insert(t.as(MTime::kSeconds), osg::AnimationPath::ControlPoint(
			getCPPosition(obj),
			getCPRotation(obj),
			getCPScale(obj)
			));
	}

	return anim;
}


/**
 *	Build the animation from Maya AnimCurves
 */
osg::ref_ptr<osg::AnimationPath> Transform::animCurve2AnimationPath(MObject &obj)
{
	osg::ref_ptr<osg::AnimationPath> anim = new osg::AnimationPath();

	// STEP 1. Get the animation curves
	MFnAnimCurve tx, ty, tz, rx, ry, rz, sx, sy, sz;

	MFnDependencyNode dn(obj);
	MPlugArray conns;
	dn.getConnections(conns);
	for(int i=0; i<conns.length(); i++){
		MPlug conn = conns[i];
		MPlugArray connectedTo;
		// Get the connections having this node as destination
		conn.connectedTo(connectedTo, true, false);
		for(int j=0; j<connectedTo.length(); j++){
			MPlug origin = connectedTo[j];
			MObject origin_node = origin.node();
			if(origin_node.hasFn(MFn::kAnimCurve)){
				if(conn.name() == dn.name() + ".translateX" ){
					tx.setObject(origin_node);
				}
				else if(conn.name() == dn.name() + ".translateY" ){
					ty.setObject(origin_node);
				}
				else if(conn.name() == dn.name() + ".translateZ" ){
					tz.setObject(origin_node);
				}
				else if(conn.name() == dn.name() + ".rotateX" ){
					rx.setObject(origin_node);
				}
				else if(conn.name() == dn.name() + ".rotateY" ){
					ry.setObject(origin_node);
				}
				else if(conn.name() == dn.name() + ".rotateZ" ){
					rz.setObject(origin_node);
				}
				else if(conn.name() == dn.name() + ".scaleX" ){
					sx.setObject(origin_node);
				}
				else if(conn.name() == dn.name() + ".scaleY" ){
					sy.setObject(origin_node);
				}
				else if(conn.name() == dn.name() + ".scaleZ" ){
					sz.setObject(origin_node);
				}
				else {
					std::cout << "Animation curve connected to parameter " << conn.name().asChar() << " (not supported)" << std::endl;
				}
			}
		}
	}

	// STEP 2. Build the AnimationPath from animation curves

	// Search the first key in time from all the AnimCurves

	bool t_present=false;
	double mint=0;

#define VALID_CURVE(f)	( f.object() != MObject::kNullObj )

#define GETMIN(f)	if(VALID_CURVE(f)) { \
									double t = f.time(0).as(MTime::kSeconds); \
									if( !t_present || t < mint ) \
										mint = t; \
									t_present = true; \
								}

	GETMIN(tx);	GETMIN(ty);	GETMIN(tz);	GETMIN(rx);	GETMIN(ry);	GETMIN(rz);	GETMIN(sx);	GETMIN(sy);	GETMIN(sz);

	// Set the right time in Maya timeline so all properties are updated
	MAnimControl::setCurrentTime(MTime(mint,MTime::kSeconds));

	// Create ControlPoint for initial time
	anim->insert(mint, osg::AnimationPath::ControlPoint(
					getCPPosition(obj),
					getCPRotation(obj),
					getCPScale(obj)
					));

	double t_prev = mint;

	// Locate the time of the next key (in any curve)

#define GET_NEXT(f)	if(VALID_CURVE(f)) { \
									for(int c=0; c<f.numKeys(); c++){ \
										double t = f.time(c).as(MTime::kSeconds); \
										if(t > t_prev && !t_present){ \
											t_now = t; \
											t_present = true; \
											break; \
										} \
										else if(t > t_prev && t < t_now) { \
											t_now = t; \
											break; \
										} \
									} \
								}

	// Get next keys cronologically
	double t_now = t_prev;
	t_present=false;
	GET_NEXT(tx); GET_NEXT(ty); GET_NEXT(tz); 
	GET_NEXT(rx); GET_NEXT(ry); GET_NEXT(rz); 
	GET_NEXT(sx); GET_NEXT(sy); GET_NEXT(sz);
	while(t_now != t_prev){

		// Set the right time in Maya timeline so all properties are updated
		MAnimControl::setCurrentTime(MTime(t_now,MTime::kSeconds));

		anim->insert(t_now, osg::AnimationPath::ControlPoint(
			getCPPosition(obj),
			getCPRotation(obj),
			getCPScale(obj)
			));

		t_prev = t_now;
		t_present=false;
		GET_NEXT(tx); GET_NEXT(ty); GET_NEXT(tz); 
		GET_NEXT(rx); GET_NEXT(ry); GET_NEXT(rz); 
		GET_NEXT(sx); GET_NEXT(sy); GET_NEXT(sz);
	}

	return anim;
}


/**
 *	Build the animation from Maya MotionPath
 */
osg::ref_ptr<osg::AnimationPath> Transform::motionPath2AnimationPath(MObject &obj)
{
	osg::ref_ptr<osg::AnimationPath> anim = new osg::AnimationPath();

	// STEP 1. Get the animation curve from this MotionPath
	MFnMotionPath motion;

	MFnDependencyNode dn(obj);
	MPlugArray conns;
	dn.getConnections(conns);
	for(int i=0; i<conns.length(); i++){
		MPlug conn = conns[i];
		MPlugArray connectedTo;
		// Get the connections having this node as destination
		conn.connectedTo(connectedTo, true, false);
		for(int j=0; j<connectedTo.length(); j++){
			MPlug origin = connectedTo[j];
			MObject origin_node = origin.node();
			if(origin_node.hasFn(MFn::kMotionPath)){
				motion.setObject(origin_node);
				break;
			}
		}
	}

	MFnAnimCurve anim_curve;
	dn.setObject(motion.object());
	dn.getConnections(conns);
	for(int i=0; i<conns.length(); i++){
		MPlug conn = conns[i];
		MPlugArray connectedTo;
		// Get the connections having this node as destination
		conn.connectedTo(connectedTo, true, false);
		for(int j=0; j<connectedTo.length(); j++){
			MPlug origin = connectedTo[j];
			MObject origin_node = origin.node();
			if(origin_node.hasFn(MFn::kAnimCurve)){
				anim_curve.setObject(origin_node);
				break;
			}
		}
	}


	// STEP 2 ...


	// STEP 3. Benefits!

	for(int i=0; i<anim_curve.numKeys(); i++){

		MTime t = anim_curve.time(i);
		double time = t.as(MTime::kSeconds);

		MAnimControl::setCurrentTime(MTime(time,MTime::kSeconds));

		anim->insert(time, osg::AnimationPath::ControlPoint(
			getCPPosition(obj),
			getCPRotation(obj),
			getCPScale(obj)
			));
	}

	return anim;
}


/**
 *	Export Transform node (and related animations)
 */
osg::ref_ptr<osg::Group> Transform::exporta(MObject &obj)
{
	MFnDependencyNode dn(obj);
	MFnTransform trfn(obj);
	MMatrix mat = trfn.transformationMatrix();

	osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform();
	osg::Matrix osgmat;
	mat.get( (double(*)[4]) osgmat.ptr() );
	trans->setMatrix(osgmat);

	/// Check if there is any animation connected to this Transform
	MFn::Type anim_type;
	if( Config::instance()->getExportAnimations() && hasAnimation(obj,anim_type)){
#ifdef _DEBUG
		std::cout << "Transform " << dn.name().asChar() << " is animated" << std::endl;
#endif

		// Check the Transform parameters
		double shear[3];
		trfn.getShear(shear);
//		std::cout << "SHEAR: " << shear[0] << " " << shear[1] << " " << shear[2] << std::endl;
		MPoint sp = trfn.scalePivot(MSpace::kTransform);
//		std::cout << "SCALE PIVOT: " << sp.x << " " << sp.y << " " << sp.z << " " << sp.w << std::endl;
		MVector spt = trfn.scalePivotTranslation(MSpace::kTransform);
//		std::cout << "SCALE PIVOT TRANSLATION: " << spt.x << " " << spt.y << " " << spt.z << std::endl;
		MPoint rp = trfn.rotatePivot(MSpace::kTransform);
//		std::cout << "ROTATE PIVOT: " << rp.x << " " << rp.y << " " << rp.z << " " << rp.w << std::endl;
		MVector rpt = trfn.rotatePivotTranslation(MSpace::kTransform);
//		std::cout << "ROTATE PIVOT TRANSLATION: " << rpt.x << " " << rpt.y << " " << rpt.z << std::endl;

		if( shear[0]!=0 || shear[1]!=0 || shear[2]!=0 ){
			std::cerr << "WARNING (" << dn.name().asChar() << ") - shear value not supported in animated Transforms (" << 
				shear[0] << ", " << shear[1] << ", " << shear[2] << ")" << std::endl;
		}
		if( sp.x != 0  || sp.y != 0 || sp.z != 0 || sp.w != 1 ){
			std::cerr << "WARNING (" << dn.name().asChar() << ") - scale pivot not supported in animated Transforms ; SP=(" << 
				sp.x << ", " << sp.y << ", " << sp.z << ", " << sp.w << ")" << std::endl;
		}
		if( spt.x != 0  || spt.y != 0 || spt.z != 0 ){
			std::cerr << "WARNING (" << dn.name().asChar() << ") - scale pivot translation not supported in animated Transforms ; SPT=(" << 
				spt.x << ", " << spt.y << ", " << spt.z << ")" << std::endl;
		}
		if( rp.x != 0  || rp.y != 0 || rp.z != 0 || rp.w != 1 ){
			std::cerr << "WARNING (" << dn.name().asChar() << ") - rotate pivot not supported in animated Transforms ; RP=(" << 
				rp.x << ", " << rp.y << ", " << rp.z << ", " << rp.w << ")" << std::endl;
		}
		if( rpt.x != 0  || rpt.y != 0 || rpt.z != 0 ){
			std::cerr << "WARNING (" << dn.name().asChar() << ") - rotate pivot translation not supported in animated Transforms ; RPT=(" << 
				rpt.x << ", " << rpt.y << ", " << rpt.z << ")" << std::endl;
		}

		// Create a callback to bind the animation to this transform
		osg::ref_ptr< osg::AnimationPath > ap;
#ifdef GENERIC_EXPORTER
		// New code to create the animation path. Independent of kind of animation.
		// It bakes any animation, not only animCurves and motionPaths, but also expressions or whatever
		ap = animatedTransform2AnimationPath(obj);
#else
		switch(anim_type){
			case MFn::kAnimCurve:
				ap = animCurve2AnimationPath(obj);
				break;
			case MFn::kMotionPath:
				ap = motionPath2AnimationPath(obj);
				break;
		}
#endif
		trans->setUpdateCallback( new osg::AnimationPathCallback( ap.get() ));
	}

	return (osg::Group *)trans.get();
}
