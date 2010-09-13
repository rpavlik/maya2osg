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
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <maya/MObject.h>

#include <osg/Node>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>


class Particle {

private:
	/**
	 *	Particle system updater
	 *
	 *	@note This node must be added at the end of the
	 *	scene graph, after emitters and programs
	 *	When it is traversed by a cull visitor, it calls 
	 *	the update() method on the specified particle systems.
	 *
	 *	@warning What to do, one updater for all or one for
	 *	each particle system? ***********
	 */
	static osg::ref_ptr<osgParticle::ParticleSystemUpdater> _updater;

public:

	/// Exports Particle node
	static osg::ref_ptr<osg::Node> exporta(MObject &obj);

	/// Get the global particle system updater
	static osg::ref_ptr<osg::Node> getParticleSystemUpdater()
	{ return _updater; }

	/// Map of particle systems
	static std::map<std::string, osgParticle::ParticleSystem *> _psMap;

	/// Clear the particle map
	static void reset()
	{ _psMap.clear(); }

};

#endif //_PARTICLE_H_
