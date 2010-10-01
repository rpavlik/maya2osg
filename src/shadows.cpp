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

#include <iostream>

#include "shadows.h"
#include "config.h"

#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ParallelSplitShadowMap>


/// Lights with shadow projection enabled
std::vector<osg::ref_ptr<osg::LightSource> > Shadows::_lightSources;

/// Resolution of the shadow map texture
int Shadows::_resolution = 1024;


/**
 *	Create the shadowing group
 */
osg::ref_ptr<osg::Group> Shadows::createShadowGroup()
{
	if ( _lightSources.empty() ) {
		return NULL;
	}

	osg::ref_ptr<osgShadow::ShadowedScene> shadowed_scene = new osgShadow::ShadowedScene();
	shadowed_scene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
	shadowed_scene->setCastsShadowTraversalMask(CastsShadowTraversalMask);

	// Decide what technique to use ...

	osgShadow::ShadowTechnique *st;

	switch ( Config::instance()->getShadowTechnique() ) {
		case Config::SHADOW_MAP:
			{
				std::cout << "ShadowMap" << std::endl;
				osgShadow::ShadowMap *sm = new osgShadow::ShadowMap();
				sm->setTextureSize(osg::Vec2s(_resolution,_resolution));
				// *** By now, only one light is supported
				sm->setLight( _lightSources[0] );
				st = sm;
			}
			break;
		case Config::SOFT_SHADOW_MAP:
			{
				std::cout << "SoftShadowMap" << std::endl;
				osgShadow::SoftShadowMap *ssm = new osgShadow::SoftShadowMap();
				ssm->setTextureSize(osg::Vec2s(_resolution,_resolution));
				// *** By now, only one light is supported
				ssm->setLight( _lightSources[0] );
				st = ssm;
			}
			break;
		case Config::SHADOW_VOLUME:
			{
				std::cout << "ShadowVolume" << std::endl;
				osgShadow::ShadowVolume *sv = new osgShadow::ShadowVolume();
				st = sv;
			}
			break;
		case Config::SHADOW_TEXTURE:
			{
				std::cout << "ShadowTexture" << std::endl;
				osgShadow::ShadowTexture *stex = new osgShadow::ShadowTexture();
				st = stex;
			}
			break;		
		case Config::PARALLEL_SPLIT_SHADOW_MAP:
			{
				std::cout << "ParallelSplitShadowMap" << std::endl;
				osgShadow::ParallelSplitShadowMap *pssm = new osgShadow::ParallelSplitShadowMap();
				pssm->setUserLight( _lightSources[0]->getLight() );
				st = pssm;
			}
			break;
	}

	shadowed_scene->setShadowTechnique(st);

	return shadowed_scene;
}

/**
 *	Add light to the list of shadow casting lights
 */
void Shadows::addLightSource( osg::ref_ptr<osg::LightSource> light, int resolution )
{
	if ( _lightSources.size() > 0 ) {
		std::cerr << "WARNING: Only one light can create shadows in current version" << std::endl;
		return;
	}
	_lightSources.push_back( light );
	_resolution = resolution;
}
