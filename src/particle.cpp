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
#include "shader.h"
#include "config.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MPlug.h>

#include <osg/Geode>


osg::ref_ptr<osgParticle::ParticleSystemUpdater> Particle::_updater = NULL;

std::map<std::string, osg::ref_ptr<osgParticle::ParticleSystem> > Particle::_psMap;

osg::ref_ptr<osg::Node> Particle::exporta(MObject &obj)
{
	MFnDependencyNode dnodefn(obj);
	MFnDagNode dagnodefn(obj);
	MFnParticleSystem particlefn(obj);
	MStatus status;

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osgParticle::ParticleSystem> particle_system = new osgParticle::ParticleSystem();
	geode->addDrawable( particle_system.get() );

	particle_system->setDefaultAttributes( "", true, false );	// *** TAKE THESE PARAMETERS FROM CONFIG

	if ( !_updater.valid() ) {
		_updater = new osgParticle::ParticleSystemUpdater();
	}
	_updater->addParticleSystem( particle_system.get() );

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
		case MFnParticleSystem::kPoints:
			{
				std::cout << "Points" << std::endl;
				particle.setShape( osgParticle::Particle::POINT );
				MPlug size = dnodefn.findPlug("pointSize",status);
				if ( !MCheckStatus(status, "finding plug pointSize") ) {
					particle.setSizeRange( osgParticle::rangef( size.asFloat(), size.asFloat() ) );
				}
				// TO-DO : Point size is not affecting OSG scene  *** FIXME!!!
			}
			break;
		case MFnParticleSystem::kStreak:
			std::cout << "Streak" << std::endl;
			particle.setShape( osgParticle::Particle::LINE );
			// TO-DO : Line width is not affecting OSG scene  *** FIXME!!!
			break;
		case MFnParticleSystem::kSprites:
			{
				std::cout << "Sprites" << std::endl;
				particle.setShape( osgParticle::Particle::QUAD );
				float scaleX = 1.0;
				float scaleY = 1.0;
				MPlug spriteScaleX = dnodefn.findPlug("spriteScaleX",status);
				if ( !MCheckStatus(status, "finding plug spriteScaleX") ) {
					scaleX = spriteScaleX.asFloat();
				}
				MPlug spriteScaleY = dnodefn.findPlug("spriteScaleY",status);
				if ( !MCheckStatus(status, "finding plug spriteScaleY") ) {
					scaleY = spriteScaleY.asFloat();
				}
				if ( scaleX != scaleY ) {
					std::cerr << "Sprite non-uniform scaling is not currently supported" << std::endl;
					std::cerr << "Using the average scale as particle size" << std::endl;
				}
				float scale = ( scaleX + scaleY ) / 2.0;
				particle.setSizeRange( osgParticle::rangef( scale, scale ) );

				// Export sprite image (file texture connected to color channel of surface shader)
				MObject shading_engine;
				Shader::getShadingEngine( obj, shading_engine );
				MObject surface_shader;
				Shader::getSurfaceShader( shading_engine, surface_shader );
				MObject texture;
				Shader::getColorTexture( surface_shader, texture );
				if ( texture.hasFn( MFn::kFileTexture ) ) {
					MFnDependencyNode dn_texture(texture);
					MPlug plug_texture_file = dn_texture.findPlug("fileTextureName");
					particle_system->setDefaultAttributes( plug_texture_file.asString().asChar(), 
						Config::instance()->getParticlesEmissive(), 
						Config::instance()->getParticlesLighting(), 0 );
				}
			}
			break;
		case MFnParticleSystem::kSpheres:
			std::cout << "Spheres" << std::endl;
			// TO-DO : User custom shapes are available in latest development OSG releases (e.g. 2.9.9)  *** FIXME!!!
			break;
		case MFnParticleSystem::kCloud:
			std::cout << "Cloud" << std::endl;
			std::cout << "CURRENTLY NOT SUPPORTED!" << std::endl;
			break;
		case MFnParticleSystem::kTube:
			std::cout << "Tube" << std::endl;
			std::cout << "CURRENTLY NOT SUPPORTED!" << std::endl;
			break;
		case MFnParticleSystem::kBlobby:
			std::cout << "Blobby" << std::endl;
			std::cout << "CURRENTLY NOT SUPPORTED!" << std::endl;
			// TO-DO : Maybe we could consider (supposed threshold is 0) to export spheres (user custom drawable)
			//	with the corresponding material/shader (instead of colour as in hardware render kSpheres)
			break;
		case MFnParticleSystem::kMultiPoint:
			std::cout << "MultiPoint" << std::endl;
			std::cout << "CURRENTLY NOT SUPPORTED!" << std::endl;
			break;
		case MFnParticleSystem::kMultiStreak:
			std::cout << "MultiStreak" << std::endl;
			std::cout << "CURRENTLY NOT SUPPORTED!" << std::endl;
			break;
		case MFnParticleSystem::kNumeric:
			std::cout << "Numeric" << std::endl;
			std::cout << "CURRENTLY NOT SUPPORTED!" << std::endl;
			break;
		default:
			std::cerr << "It is impossible you reach this point in the code, because all options for particle render type were checked" << std::endl;
			break;
	}

	// Radius
	MPlug radius = dnodefn.findPlug("radius", &status);
	if ( !MCheckStatus(status, "finding plug radius") ) {
		// *** CHECK RADIUS RAMPS/EXPRESSIONS...  FIXME!!!
		particle.setRadius( radius.asFloat() );
	}

	// Color range/color interpolator
	osg::Vec4 color;
	MPlug colorPlug;
	bool hasColor = true;
	colorPlug = dnodefn.findPlug("colorRed", &status);
	if ( !MCheckStatus(status, "finding plug colorRed") )
		color.r() = colorPlug.asDouble();
	else
		hasColor = false;
	colorPlug = dnodefn.findPlug("colorGreen", &status);
	if ( !MCheckStatus(status, "finding plug colorGreen") )
		color.g() = colorPlug.asDouble();
	else
		hasColor = false;
	colorPlug = dnodefn.findPlug("colorBlue", &status);
	if ( !MCheckStatus(status, "finding plug colorBlue") )
		color.b() = colorPlug.asDouble();
	else
		hasColor = false;
	if ( hasColor ) {
		color.a() = 1.0;
		particle.setColorRange( osgParticle::rangev4( color, color ) );
	}

	// Alpha range
	MPlug opacity = dnodefn.findPlug("opacity", &status);
	if ( !MCheckStatus(status, "finding plug opacity") ) {
		particle.setAlphaRange( osgParticle::rangef( opacity.asFloat(), opacity.asFloat() ) );
	}

	// Mass
//	particle.setMass();		// *** Maya only has per-particle (not per-object) mass

	particle_system->setDefaultParticleTemplate( particle );

	// Particle systems are registered in a map to be able to
	// attach the emitters later
	std::cout << dnodefn.name().asChar() << " " << dagnodefn.fullPathName().asChar() << std::endl;
	_psMap[ dagnodefn.fullPathName().asChar() ] = particle_system;

	return geode;
}
