#include <maya/MFnCamera.h>
#include <maya/MColor.h>
#include <maya/MPlug.h>
#include "camera.h"
#include "config.h"
#include <fstream>
#include <osgViewer/Viewer>
#include <osgDB/WriteFile>

std::string Camera::_sceneFileBaseName;

/**
 *	Exports Camera node
 */
osg::ref_ptr<osg::Node> Camera::exporta(MObject &obj)
{
	// Each camera is exported to a separate file
	MFnCamera cam(obj);
	MFnDagNode dn(obj);

	std::string node_name = dn.name().asChar();
	if ( !Config::instance()->getExportDefaultCameras() && (
		node_name == "topShape" ||
		node_name == "frontShape" ||
		node_name == "sideShape" ||
		node_name == "perspShape"
		)) 
	{
		return NULL;
	}

	if ( ! Config::instance()->getExportOrthographicCameras() && cam.isOrtho() )
		return NULL;

	osgViewer::Viewer viewer;

	// Camera name
	viewer.setName(dn.name().asChar());
//	std::cout << "Exporting Camera " << dn.name().asChar() << std::endl;

	// Background color
	MPlug backgroundColor = cam.findPlug("backgroundColor");
	float red, green, blue;
	backgroundColor.child(0).getValue( red );
	backgroundColor.child(1).getValue( green );
	backgroundColor.child(2).getValue( blue );
	viewer.getCamera()->setClearColor( osg::Vec4( red, green, blue, 1.0 ) );

	// View matrix (does not work as expected. Transformations are not considered. FIXME!)
	MPoint meye = cam.eyePoint(MSpace::kPostTransform);
	MPoint mcenter = cam.centerOfInterestPoint(MSpace::kPostTransform);
	MPoint mup = cam.upDirection(MSpace::kPostTransform);
//	std::cout << "eye    : " << meye.x << " " << meye.y << " " << meye.z << std::endl;
//	std::cout << "center : " << mcenter.x << " " << mcenter.y << " " << mcenter.z << std::endl;
//	std::cout << "up     : " << mup.x << " " << mup.y << " " << mup.z << std::endl;
	osg::Vec3d eye(meye.x, meye.y, meye.z);
	osg::Vec3d center(mcenter.x, mcenter.y, mcenter.z);
	osg::Vec3d up(mup.x, mup.y, mup.z);
	viewer.getCamera()->setViewMatrixAsLookAt( eye, center, up );

	// Projection matrix
	double l, r, b ,t;
	// We use the rendering frustum instead of the viewing frustum
	cam.getRenderingFrustum(cam.aspectRatio(), l, r, b, t);
	double n = cam.nearClippingPlane();
	double f = cam.farClippingPlane();
	if ( cam.isOrtho() ) {
		viewer.getCamera()->setProjectionMatrixAsOrtho(l, r, b, t, n, f);
	}
	else {
		viewer.getCamera()->setProjectionMatrixAsFrustum(l, r, b, t, n, f);
	}

	// Auto Render Clip Plane -> ComputeNearFarMode
	MPlug bfcp = cam.findPlug("bestFitClippingPlanes");
	bool autoClipPlanes;
	bfcp.getValue(autoClipPlanes);
	if ( !autoClipPlanes ) {
		viewer.getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	}

	// Write camera/viewer config file
	std::string camera_filename = _sceneFileBaseName + "_" + std::string(dn.name().asChar()) + ".osg";
	osgDB::writeObjectFile( viewer, camera_filename );

	// We do not include the camera in the scene graph, so return NULL
	return NULL;
}

/// Get the scene file base name
const std::string &Camera::getSceneFileBaseName()
{ return _sceneFileBaseName; }

/// Set the scene file base name
void Camera::setSceneFileBaseName( const std::string &name )
{ _sceneFileBaseName = name; }
