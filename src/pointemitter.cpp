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

#include "pointemitter.h"
#include "particle.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>

#include <osgParticle/ConstantRateCounter>
#include <osgParticle/RadialShooter>
#include <osg/MatrixTransform>


std::vector< std::pair<osg::ref_ptr<osgParticle::ModularEmitter>, std::vector<std::string> > > PointEmitter::_emittersParticles;


osg::ref_ptr<osg::Node> PointEmitter::exporta(MObject &obj)
{
	MFnDependencyNode dnodefn(obj);

	osg::ref_ptr<osg::MatrixTransform> trans_emitter = NULL;

	// What we need to create the OSG emitter (osgParticle::ModularEmitter) :
	osg::ref_ptr<osgParticle::ModularEmitter> emitter = new osgParticle::ModularEmitter();

	// Configure the emitter properties

	// Counter (controls the number of particles to be emitted at each frame)
	MPlug rate = dnodefn.findPlug("rate");
	osg::ref_ptr<osgParticle::ConstantRateCounter> counter = new osgParticle::ConstantRateCounter();
	counter->setNumberOfParticlesPerSecondToCreate( rate.asDouble() );
	emitter->setCounter( counter );

	// Placer (initialize particle's position vector)

	// ... for now we leave it as default (create particles in the position of the emitter)

	// Shooter (initialize particle's velocity vector)
	float speed = dnodefn.findPlug("speed").asFloat();
	float speedRandom = dnodefn.findPlug("speedRandom").asFloat();
	osg::ref_ptr<osgParticle::RadialShooter> shooter = new osgParticle::RadialShooter();
	// Speed
	shooter->setInitialSpeedRange( speed - speedRandom, speed + speedRandom );
	switch ( dnodefn.findPlug("emitterType").asInt() ) {
		case 0:	// Directional
			std::cout << "Directional" << std::endl;
			{
				float spread = dnodefn.findPlug("spread").asFloat();
				// Theta range
				shooter->setThetaRange( 0, spread * M_PI / 2.0 );
				// Phi range
				shooter->setPhiRange( -M_PI, M_PI );
				// Rotate the emitter to fit the emission direction in Maya
				osg::Vec3 direction( dnodefn.findPlug("directionX").asFloat(),
					dnodefn.findPlug("directionY").asFloat(),
					dnodefn.findPlug("directionZ").asFloat() );
				trans_emitter = new osg::MatrixTransform();
				osg::Matrix mat;
				mat.makeRotate( osg::Vec3(0.0, 0.0, 1.0), direction );
				trans_emitter->setMatrix( mat );
				trans_emitter->addChild( emitter );
			}
			break;
		case 1:	// Omni
			std::cout << "Omni" << std::endl;
			// Theta range
			shooter->setThetaRange( 0, M_PI );
			// Phi range
			shooter->setPhiRange( -M_PI, M_PI );
			break;
		case 2:	// Surface
			std::cout << "Surface" << std::endl;
			std::cout << "NOT SUPPORTED" << std::endl;
			return NULL;
			break;
		case 3:	// Curve
			std::cout << "Curve" << std::endl;
			std::cout << "NOT SUPPORTED" << std::endl;
			return NULL;
			break;
		case 4:	// Volume
			std::cout << "Volume" << std::endl;
			std::cout << "NOT SUPPORTED" << std::endl;
			return NULL;
			break;
	}
	// InitialRotationalSpeed range *** TO-DO
	emitter->setShooter( shooter );

	emitter->setName( dnodefn.name().asChar() );

	// Particle systems connected to this emitter
	std::vector<std::string> particle_systems_names;

	MPlugArray conns;
	dnodefn.getConnections(conns);
	for(int i=0; i<conns.length(); i++){
		MPlug conn = conns[i];
		MPlugArray connectedTo;
		// Get the connections having this node as source
		conn.connectedTo(connectedTo, false, true);
		for(int j=0; j<connectedTo.length(); j++){
			MPlug destination = connectedTo[j];
			MObject destination_node = destination.node();
			if(destination_node.hasFn(MFn::kParticle)){
				MFnDependencyNode particle(destination_node);
				particle_systems_names.push_back( particle.name().asChar() );
				std::cout << "CONNECTION BETWEEN " << dnodefn.name().asChar() << " AND " << particle.name().asChar() << std::endl;
			}
		}
	}
	std::pair<osgParticle::ModularEmitter *, std::vector<std::string> > pair ( emitter, particle_systems_names );
	_emittersParticles.push_back( pair );

	if ( trans_emitter.valid() )
		return trans_emitter;
	else
		return emitter;
}


/**
 *	Bind emitters to particle systems 
 *	This operation must be done at the end of the export,
 *	when all nodes exist because emitters and partiles
 *	can be placed in any order in the DAG
 */
void PointEmitter::bindEmittersToParticles()
{
	for( int e=0 ; e < _emittersParticles.size() ; e++ ) {
		osgParticle::ModularEmitter *em = _emittersParticles[e].first;
		for( int p=0 ; p < _emittersParticles[e].second.size() ; p++ ) {
			std::string ps_name = _emittersParticles[e].second[p];
			if ( Particle::_psMap.find(ps_name) == Particle::_psMap.end() ) {
				std::cerr << "ERROR: Particle object " << ps_name << " not found" << std::endl;
				continue;
			}
			osgParticle::ParticleSystem *ps = Particle::_psMap[ps_name];
			std::cout << "Binding particle '" << ps_name << "' to emitter " << em->getName() << std::endl;
			em->setParticleSystem( ps );
		}
	}
}
