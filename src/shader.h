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
#ifndef _SHADER_H_
#define _SHADER_H_

#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <osg/StateSet>
#include <osg/Material>

class Shader {

public:
	static osg::ref_ptr<osg::StateSet> exporta(MObject &obj, MObjectArray &textures);

private:
	static osg::ref_ptr<osg::Material> material(MObject &obj, bool &mat_trans);
	static void setTextures(osg::ref_ptr<osg::StateSet> st, MObjectArray &textures, bool tex_trans);
	/// Check if the material has any texture connected to the specified channel
	static bool connectedTexture(MObject &obj, std::string channel);

};

#endif //_SHADER_H_
