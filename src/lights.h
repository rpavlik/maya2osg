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
#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include <vector>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/StateSet>
#include <osg/LightModel>

/**
 *	This class manages the lights in the scene
 *
 *	It keeps a list of the existing lights to activate them in the root node StateSet
 */
class Lights {

private:

	static std::vector< osg::ref_ptr<osg::Light> > _lights;	///< Lights present in the scene

	/// Uniform indicating the number of lights in the scene
	static osg::ref_ptr<osg::Uniform> _uniformNumEnabledLights;

public:

	/// Registers a light in the scene (and creates the LightSource node)
	static osg::ref_ptr<osg::LightSource> registerLight(osg::ref_ptr<osg::Light> light);

	/// Configure the global stateset to use the defined lights
	static void configureStateSet(osg::ref_ptr<osg::StateSet> ss);

	/// Configure uniforms needed for lighting
	static void configureUniforms();

	static osg::ref_ptr<osg::Uniform> getUniformNumEnabledLights()
	{ return _uniformNumEnabledLights; }

	/// Empty the lights list (leaving the system ready to export the next scene)
	static inline void reset()
	{ _lights.clear(); }

	/// Get a LightModel that follow default configuration
	static osg::ref_ptr<osg::LightModel> getDefaultLightModel();

};

#endif //_LIGHTS_H_
