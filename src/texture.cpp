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
#include "texture.h"
#include "config.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MFnStringData.h>
#include <maya/MPlug.h>

#include <osgDB/FileNameUtils>
#include <osgDB/ReadFile>

// Static maps (texture cache)
Texture::TTexturesMap Texture::texMap;
Texture::TImagesMap Texture::imagesMap;


/**
 *	Delete texture and images caches
 */
void Texture::deleteCaches()
{
	texMap.clear();
	imagesMap.clear();
}


/**
 *	Export the texture
 */
osg::ref_ptr<osg::Texture2D> Texture::exporta(const MObject &obj, const std::string filename)
{
	MPlug plug;
	MString texname;

	MFnDependencyNode dn(obj);
    if ( filename.empty() ) {
	    plug = dn.findPlug("fileTextureName");
	    plug.getValue(texname);
    }
    else {
        texname = filename.c_str();
    }

	// *************************************************************
	// ******* TODO: Extract the path before "sourceimages" ********
	// *************************************************************

	// First we search the texture in the cache to avoid duplicates
	TTexturesMap::const_iterator it = texMap.find( dn.name().asChar() );
	if( it != texMap.end() ){
		return it->second;
	}

	// Image (is also searched in cache)
	osg::ref_ptr<osg::Image> image;
	TImagesMap::const_iterator iti = imagesMap.find( texname.asChar() );
	if( iti != imagesMap.end() ){
		image = iti->second;
	}
	else {
		image = new osg::Image();
		image->setFileName(texname.asChar());
		// Store image in the map for next searches
		imagesMap[texname.asChar()] = image;
	}

	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D(image.get());

	// Texture filtering
	MPlug filterType = dn.findPlug("filterType");
	switch( filterType.asInt() ) {
		case 0: // No filter
			tex->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST );
			tex->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST );
			break;
		case 1:	// Mip Map
		case 2:	// Box
		case 3:	// Quadratic
		case 4:	// Quartic
		case 5:	// Gaussian
			tex->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR );
			tex->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
            tex->setUseHardwareMipMapGeneration( true );
			break;
	}

	// Border color -> default color
	if ( Config::instance()->getTexClampMode() == Config::COLOR ) {
		MPlug defaultColorR = dn.findPlug("defaultColorR");
		MPlug defaultColorG = dn.findPlug("defaultColorG");
		MPlug defaultColorB = dn.findPlug("defaultColorB");
		osg::Vec4 border_color( defaultColorR.asFloat(), defaultColorG.asFloat(), defaultColorB.asFloat(), 1.0 );
		tex->setBorderColor( border_color );
	}

	// Wrapping
	bool test;
	plug = dn.findPlug("mirrorU");
	plug.getValue(test);
	if(test){
		tex->setWrap(osg::Texture::WRAP_S, osg::Texture::MIRROR);
	}
	else{
		plug = dn.findPlug("wrapU");
		plug.getValue(test);
		if(test){
			tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
		}
		else{
			switch( Config::instance()->getTexClampMode() ) {
				case Config::EDGE:
					tex->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
					break;
				case Config::COLOR:
					tex->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
					break;
			}
		}
	}
	plug = dn.findPlug("mirrorV");
	plug.getValue(test);
	if(test){
		tex->setWrap(osg::Texture::WRAP_T, osg::Texture::MIRROR);
	}
	else{
		plug = dn.findPlug("wrapV");
		plug.getValue(test);
		if(test){
			tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
		}
		else{
			switch( Config::instance()->getTexClampMode() ) {
				case Config::EDGE:
					tex->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
					break;
				case Config::COLOR:
					tex->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
					break;
			}
		}
	}

	// Anisotropic filtering
	int max_aniso = Config::instance()->getMaxAnisotropy();
	if ( max_aniso > 0 ) {
		tex->setMaxAnisotropy( max_aniso );
	}

	// Se almacena en el mapa de texturas para las siguientes búsquedas
	texMap[dn.name().asChar()] = tex;

	return tex;
}


/**
 *	Export the texture matrix
 */
osg::ref_ptr<osg::TexMat> Texture::exportTexMat(const MObject &obj)
{
	MPlug plug;
	MFnDependencyNode dn(obj);

	osg::Matrix mat;

	float repeatU, repeatV;
	plug = dn.findPlug("repeatU");
	plug.getValue(repeatU);
	plug = dn.findPlug("repeatV");
	plug.getValue(repeatV);
	mat.makeScale(repeatU, repeatV, 1.0);

	float ang;
	plug = dn.findPlug("rotateUV");
	plug.getValue(ang);
	mat.postMult(osg::Matrix::translate(-0.5,-0.5,0));
	mat.postMult(osg::Matrix::rotate(ang,0,0,1));
	mat.postMult(osg::Matrix::translate(0.5,0.5,0));

	float offsU, offsV;
	plug = dn.findPlug("offsetU");
	plug.getValue(offsU);
	plug = dn.findPlug("offsetV");
	plug.getValue(offsV);
	mat.postMult(osg::Matrix::translate(offsU, offsV, 0));

	if( mat == osg::Matrix::identity() )
		return NULL;

	osg::TexMat *texmat = new osg::TexMat();
	texmat->setMatrix(mat);
	return texmat;
}
