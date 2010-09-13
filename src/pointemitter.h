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
#ifndef _POINTEMITTER_H_
#define _POINTEMITTER_H_

#include <maya/MObject.h>

#include <osg/Node>
#include <osgParticle/ModularEmitter>


class PointEmitter {

private:
	/// Association to each emitter the particle systems that emits
	static std::vector<std::pair<osgParticle::ModularEmitter *, std::vector<std::string> > > _emittersParticles;

public:
	/// Exports Emitter node
	static osg::ref_ptr<osg::Node> exporta(MObject &obj);

	/**
	 *	Bind emitters to particle systems 
	 *	This operation must be done at the end of the export,
	 *	when all nodes exist because emitters and partiles
	 *	can be placed in any order in the DAG
	 */
	static void bindEmittersToParticles();

	/// Clear the particle-emitter bindings
	static void reset()
	{ _emittersParticles.clear(); }

};

#endif //_POINTEMITTER_H_
