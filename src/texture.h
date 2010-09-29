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
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <maya/MObject.h>
#include <osg/Texture2D>
#include <osg/TexMat>
#include <map>
#include <string>

/**
 *	This class implement a texture cache to reuse them
 *	in every instance in the exported model, avoiding duplicates.
 */
class Texture {

public:
	/// Export textures in this Maya Object
	static osg::ref_ptr<osg::Texture2D> exporta(const MObject &obj);

	/// Export texture matrix of this object
	static osg::ref_ptr<osg::TexMat> exportTexMat(const MObject &obj);

	/// Delete texture and images cache
	static void deleteCaches();

private:
	typedef std::map< std::string, osg::ref_ptr< osg::Texture2D > > TTexturesMap;
	/// OSG textures map
	static TTexturesMap texMap;
	typedef std::map< std::string, osg::ref_ptr< osg::Image > > TImagesMap;
	/// OSG images map (used by textures)
	static TImagesMap imagesMap;

};

#endif //_TEXTURE_H_
