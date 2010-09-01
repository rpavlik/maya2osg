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
#ifndef _PROXYMATERIALS_H_
#define _PROXYMATERIALS_H_

#include <maya/MGlobal.h>

#include <osg/StateSet>

#include <map>

class ProxyMaterials {

private:
	/// Map of Maya shaders bound to OSG StateSets
	static std::map<void *, MObject> _shaderMap;

public:

	/// Get the shader for a StateSet
	static MObject &getShader(osg::StateSet *ss);	

	/// Empty the map
	static void clear() 	{ _shaderMap.clear(); }

};

#endif //_PROXYMATERIALS_H_
