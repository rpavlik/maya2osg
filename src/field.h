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
#ifndef _FIELD_H_
#define _FIELD_H_

#include <vector>

#include <maya/MObject.h>

class Field {

public:

	/// Association of each field to the particle systems that affects
	static std::vector<std::pair<Field, std::vector<std::string> > > _fieldsParticles;

	/// Exports a Maya field
	static void exporta( const MObject &field );

	/**
	 *	Bind fields to particle systems 
	 *	This operation must be done at the end of the export,
	 *	when all nodes exist because fields and partiles
	 *	can be placed in any order in the DAG
	 */
	static void bindFieldsToParticles();

};

#endif //_FIELD_H_
