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
#include "spotlight.h"
#include "lights.h"
#include "config.h"
#include "shadows.h"

#include <maya/MFnLight.h>
#include <maya/MFnNonAmbientLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MColor.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

#include <iostream>

osg::ref_ptr<osg::Node> SpotLight::exporta(MObject &obj)
{
	MFnSpotLight mslight(obj);
	MFnDependencyNode dn(obj);

	osg::Light *light = new osg::Light();

	// Position
	light->setPosition(osg::Vec4(0,0,0,1));
	light->setDirection(osg::Vec3(0,0,-1));

	// Ambient, diffuse and specular components
	MColor color = mslight.color();
	float intensity = mslight.intensity();
	if(mslight.lightAmbient())
		light->setAmbient(osg::Vec4( color.r*intensity, color.g*intensity, color.b*intensity, color.a ));
	else
		light->setAmbient(osg::Vec4(0,0,0,1));
	if(mslight.lightDiffuse())
		light->setDiffuse(osg::Vec4( color.r*intensity, color.g*intensity, color.b*intensity, color.a ));
	else
		light->setDiffuse(osg::Vec4(0,0,0,1));
	if(mslight.lightSpecular())
		light->setSpecular(osg::Vec4( color.r*intensity, color.g*intensity, color.b*intensity, color.a ));
	else
		light->setSpecular(osg::Vec4(0,0,0,1));

	switch(mslight.decayRate()) {
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
			std::cerr << "Unknown Decay Rate type : " << mslight.decayRate() << std::endl;
			break;
	}

	// Spot light parameters
	light->setSpotCutoff( osg::RadiansToDegrees( mslight.coneAngle() ) / 2.0 );
	// *** mslight.penumbraAngle();
	light->setSpotExponent( mslight.dropOff() );

	osg::ref_ptr<osg::LightSource> light_source = Lights::registerLight(light);
    light_source->getLight()->setName(mslight.name().asChar());

	// Shadow casting
	if ( Config::instance()->getComputeShadows() ) {
		if ( dn.findPlug("useDepthMapShadows").asBool() ) {
			std::cout << "Light " << dn.name().asChar() << " useDepthMapShadows with resolution " << dn.findPlug("dmapResolution").asInt() << std::endl;
			Shadows::addLightSource( light_source, dn.findPlug("dmapResolution").asInt() );
		}
	}

	return light_source;
}
