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
#ifndef _OSGWRITE_H_
#define _OSGWRITE_H_


#include <maya/MStatus.h>
#include <maya/MString.h>


class OSGWrite {

public:
	/// parses Arguments and set config class member data
	static MString parseArgs( const MStringArray & argStringArray ) ;

	/// starts exporting process, extracted here to use in maya2osg command and osg file translator
	static MStatus exporta( const MString & filename ) ;
	

};

#endif //_OSGWRITE_H_
