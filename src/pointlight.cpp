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
#include "pointlight.h"
#include "lights.h"
#include "shadows.h"
#include "config.h"

#include <maya/MFnLight.h>
#include <maya/MFnNonAmbientLight.h>
#include <maya/MColor.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

#include <iostream>

osg::ref_ptr<osg::Node> PointLight::exporta(MObject &obj)
{
	MFnDependencyNode dn(obj);
	MFnLight mlight(obj);
	MFnNonAmbientLight mnalight(obj);

	osg::Light *light = new osg::Light();

	// Position (point light => local => omnidirectional)
	light->setPosition(osg::Vec4(0,0,0,1));

	// Ambient, diffuse and specular components
	MColor color = mlight.color();
	float intensity = mlight.intensity();
	if(mlight.lightAmbient())
		light->setAmbient(osg::Vec4( color.r*intensity, color.g*intensity, color.b*intensity, color.a ));
	else
		light->setAmbient(osg::Vec4(0,0,0,1));
	if(mlight.lightDiffuse())
		light->setDiffuse(osg::Vec4( color.r*intensity, color.g*intensity, color.b*intensity, color.a ));
	else
		light->setDiffuse(osg::Vec4(0,0,0,1));
	if(mlight.lightSpecular())
		light->setSpecular(osg::Vec4( color.r*intensity, color.g*intensity, color.b*intensity, color.a ));
	else
		light->setSpecular(osg::Vec4(0,0,0,1));

	switch(mnalight.decayRate()) {
		case 0:
			// No decay (constant)
			light->setConstantAttenuation(1.0f);
			light->setLinearAttenuation(0.0f);
			light->setQuadraticAttenuation(0.0f);
			break;
		case 1:
			// Linear
			light->setConstantAttenuation(0.0f);
			light->setLinearAttenuation(1.0f);
			light->setQuadraticAttenuation(0.0f);
			break;
		case 2:
			// Quadratic
			light->setConstantAttenuation(0.0f);
			light->setLinearAttenuation(0.0f);
			light->setQuadraticAttenuation(1.0f);
			break;
		case 3:
			// Cubic
			std::cerr << "Cubic Decay Rate not supported in OpenGL" << std::endl;
			break;
		default:
			std::cerr << "Unknown Decay Rate type : " << mnalight.decayRate() << std::endl;
			break;
	}

	osg::ref_ptr<osg::LightSource> light_source = Lights::registerLight(light);

	// Shadow casting
	if ( Config::instance()->getComputeShadows() ) {
		if ( dn.findPlug("useDepthMapShadows").asBool() ) {
			std::cout << "Light " << dn.name().asChar() << " useDepthMapShadows with resolution " << dn.findPlug("dmapResolution").asInt() << std::endl;
			Shadows::addLightSource( light_source, dn.findPlug("dmapResolution").asInt() );
		}
	}

	return light_source;
}
