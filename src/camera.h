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
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <osg/Node>

class Camera {

private:
	/// Scene file base name
	static std::string _sceneFileBaseName;

public:
	/// Exports Camera node
	static osg::ref_ptr<osg::Node> exporta(MObject &obj);

	/// Get the scene file base name
	static const std::string &getSceneFileBaseName();

	/// Set the scene file base name
	static void setSceneFileBaseName( const std::string &name );

};

#endif //_CAMERA_H_