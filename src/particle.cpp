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

#include "particle.h"
#include "common.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MPlug.h>

#include <osg/Geode>


osg::ref_ptr<osgParticle::ParticleSystemUpdater> Particle::_updater = NULL;

std::map<std::string, osgParticle::ParticleSystem *> Particle::_psMap;

osg::ref_ptr<osg::Node> Particle::exporta(MObject &obj)
{
	MFnDependencyNode dnodefn(obj);
	MFnParticleSystem particlefn(obj);
	MStatus status;

	osg::Geode *geode = new osg::Geode();
	osgParticle::ParticleSystem *particle_system = new osgParticle::ParticleSystem();
	geode->addDrawable( particle_system );

	particle_system->setDefaultAttributes( "", true, false );

	if ( !_updater.valid() ) {
		_updater = new osgParticle::ParticleSystemUpdater();
	}
	_updater->addParticleSystem( particle_system );

	osgParticle::Particle particle;

	// Set OSG particle properties from Maya particle properties
	
	// Lifetime
	if ( particlefn.hasLifespan() ) {
		MPlug lifespanMode = dnodefn.findPlug("lifespanMode", &status);
		if ( !MCheckStatus(status, "finding plug lifespanMode") ) {
			switch ( lifespanMode.asInt() ) {
				case 0: // Live forever
					std::cout << "Live forever" << std::endl;
					particle.setLifeTime( -1.0 );
					break;
				case 1:	// Constant
					{
						MPlug lifespan = dnodefn.findPlug("lifespan", &status);
						if ( !MCheckStatus(status, "finding plug lifespan") ) {
							std::cout << "Constant - lifespan : " << lifespan.asFloat() << std::endl;
							particle.setLifeTime( lifespan.asFloat() );
						}
					}
					break;
				case 2:	// Random range
					{
						MPlug lifespanRandom = dnodefn.findPlug("lifespanRandom", &status);
						if ( !MCheckStatus(status, "finding plug lifespanRandom") ) {
							MPlug lifespan = dnodefn.findPlug("lifespan", &status);
							if ( !MCheckStatus(status, "finding plug lifespan") ) {
								std::cout << "Random range - lifespan : " << lifespan.asFloat() << " lifespanRandom : " << lifespanRandom.asFloat() << std::endl;
								particle.setLifeTime( lifespan.asFloat() );
								// TO-DO *** RANDOM RANGE - FIXME!!!!
							}
						}
					}
					break;
				case 3:	// lifespanPP only
					std::cout << "lifespanPP only" << std::endl;
					std::cerr << "lifespanPP NOT SUPPORTED!" << std::endl;
					particle.setLifeTime( -1.0 );
					break;
			}
		}
	}
	else {
		particle.setLifeTime( -1.0 );
	}

	switch( particlefn.renderType() ) {
		case MFnParticleSystem::kCloud:
			std::cout << "Cloud" << std::endl;
			break;
		case MFnParticleSystem::kTube:
			std::cout << "Tube" << std::endl;
			break;
		case MFnParticleSystem::kBlobby:
			std::cout << "Blobby" << std::endl;
			break;
		case MFnParticleSystem::kMultiPoint:
			std::cout << "MultiPoint" << std::endl;
			break;
		case MFnParticleSystem::kMultiStreak:
			std::cout << "MultiStreak" << std::endl;
			break;
		case MFnParticleSystem::kNumeric:
			std::cout << "Numeric" << std::endl;
			break;
		case MFnParticleSystem::kPoints:
			std::cout << "Points" << std::endl;
			break;
		case MFnParticleSystem::kSpheres:
			std::cout << "Spheres" << std::endl;
			break;
		case MFnParticleSystem::kSprites:
			std::cout << "Sprites" << std::endl;
			break;
		case MFnParticleSystem::kStreak:
			std::cout << "Streak" << std::endl;
			break;
		default:
			std::cerr << "It is impossible you reach this point in the code, because all options for particle render type were checked" << std::endl;
			break;
	}

	// Size/radius
	MPlug radius = dnodefn.findPlug("radius", &status);
	MCheckStatus(status, "finding plug radius");
	// *** CHECK RADIUS RAMPS/EXPRESSIONS...  FIXME!!!
	particle.setRadius( radius.asFloat() );

	// TO-DO:
	// Size range, size interpolator, ...
	// Color range, color interpolator, ...
	// Mass
	// ...

	particle_system->setDefaultParticleTemplate( particle );

	// Particle systems are registered in a map to be able to
	// attach the emitters later
	_psMap[ dnodefn.name().asChar() ] = particle_system;

	return geode;
}
