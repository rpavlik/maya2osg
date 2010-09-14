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


std::vector< std::pair<osgParticle::ModularEmitter *, std::vector<std::string> > > PointEmitter::_emittersParticles;


osg::ref_ptr<osg::Node> PointEmitter::exporta(MObject &obj)
{
	MFnDependencyNode dnodefn(obj);

	// What we need to create the OSG emitter (osgParticle::ModularEmitter) :
	osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter();

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

	// Configure the emitter properties

	// Counter (controls the number of particles to be emitted at each frame)
	MPlug rate = dnodefn.findPlug("rate");
	osgParticle::ConstantRateCounter *counter = new osgParticle::ConstantRateCounter();
	counter->setNumberOfParticlesPerSecondToCreate( rate.asDouble() );
	emitter->setCounter( counter );

	// Placer (initialize particle's position vector)

	// ... for now we leave it as default (create particles in the position of the emitter)

	// Shooter (initialize particle's velocity vector)
	float speed = dnodefn.findPlug("speed").asFloat();
	float speedRandom = dnodefn.findPlug("speedRandom").asFloat();
	osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter();
	shooter->setInitialSpeedRange( speed - speedRandom, speed + speedRandom );
	// Theta range ***
	// Phi range ***
	// InitialRotationalSpeed range ***
	emitter->setShooter( shooter );

	emitter->setName( dnodefn.name().asChar() );

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
