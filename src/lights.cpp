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
#include "lights.h"
#include "config.h"

#include <iostream>

// Lights list
std::vector< osg::ref_ptr<osg::Light> > Lights::_lights;

/// Uniform indicating the number of lights in the scene
osg::ref_ptr<osg::Uniform> Lights::_uniformNumEnabledLights = new osg::Uniform(osg::Uniform::INT, "NumEnabledLights");

/**
 *	Register a light in the scene (and create the LightSource node)
 */
osg::ref_ptr<osg::LightSource> Lights::registerLight(osg::ref_ptr<osg::Light> light)
{
	if ( ! light.valid() )
		return NULL;

	// Query the maximum number of ligths allowed in this OpenGL implementation
	int max_lights_opengl;
	glGetIntegerv(GL_MAX_LIGHTS, &max_lights_opengl);

	if (_lights.size() >= max_lights_opengl){
		std::cout << "ERROR. No more than " << max_lights_opengl << " lights allowed" << std::endl;
		std::cout << "The light is discarded" << std::endl;
		return NULL;
	}

	light->setLightNum(_lights.size());
	_lights.push_back(light); 

	osg::LightSource *ls = new osg::LightSource();
	ls->setLight(light.get());

	return ls;
}

/**
 *	Configure the global stateset to use the lights defined in the scene
 */
void Lights::configureStateSet(osg::ref_ptr<osg::StateSet> ss)
{
	if( _lights.size() > 0 )
	{
		ss->setMode(GL_LIGHTING, osg::StateAttribute::ON);
		ss->setAttribute( getDefaultLightModel().get() );

		for(int i=0; i < _lights.size(); i++){
			ss->setMode(GL_LIGHT0+i, osg::StateAttribute::ON);
		}
	}
}

/**
 *	Get a LightModel that follow default configuration
 */
osg::ref_ptr<osg::LightModel> Lights::getDefaultLightModel()
{
	osg::ref_ptr<osg::LightModel> lm = new osg::LightModel();
	lm->setAmbientIntensity( osg::Vec4(Config::instance()->getGlobalAmbient(), 1.0) );
	lm->setLocalViewer( Config::instance()->getLocalViewer() );
	lm->setColorControl(osg::LightModel::SINGLE_COLOR);
	lm->setTwoSided(false);
	return lm;
}


/**
 *	Configure uniforms needed for lighting
 */
void Lights::configureUniforms()
{
	_uniformNumEnabledLights->set( (int) _lights.size() );
}

