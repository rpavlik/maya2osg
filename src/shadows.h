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
#ifndef _SHADOWS_H_
#define _SHADOWS_H_

#include <osg/Group>
#include <osg/LightSource>
#include <osgShadow/ShadowedScene>

class Shadows {

private:

	/// Resolution of the shadow map texture
	static int _resolution;

	/// Lights with shadow projection enabled
	static std::vector<osg::ref_ptr<osg::LightSource> > _lightSources;

public:

	static const int ReceivesShadowTraversalMask = 0x1;
	static const int CastsShadowTraversalMask = 0x2;

	/// Create the shadow group (the exported scene will be child of this group)
	static osg::ref_ptr<osg::Group> createShadowGroup();

	/// Add light to the list of shadow casting lights
	static void addLightSource( osg::ref_ptr<osg::LightSource> light, int resolution = 1024 );

	/// Clear registered lights
	inline static void reset()
	{ _lightSources.clear(); }

};

#endif //_SHADOWS_H_
