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

#include "field.h"


/// Exports a Maya field
void Field::exporta( const MObject &field )
{
	if ( field.hasFn( MFn::kGravity ) ) {
		// Add the field to the registry with all its connections
//		_fieldsParticles
		// ... **** FIXME!!!
	}

}


/**
 *	Bind fields to particle systems 
 *	This operation must be done at the end of the export,
 *	when all nodes exist because fields and particles
 *	can be placed in any order in the DAG
 */
void Field::bindFieldsToParticles()
{
/*
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
*/
}
