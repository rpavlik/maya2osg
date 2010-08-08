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
#include "shader.h"
#include "texture.h"
#include "config.h"

#include <iostream>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MColor.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/BlendFunc>


/**
 *	Check whether there is any texture connected to any channel of this material (color, transparency, ...)
 */
bool Shader::connectedTexture(MObject &obj, std::string canal)
{
	MFnDependencyNode dn(obj);
	// Maya materials inherit from Lambert node, so if it is not a lambert, we do not consider it a material
	if(obj.hasFn(MFn::kLambert)){
		MFnLambertShader lambert(obj);

		MStatus status;
		MPlug plug = dn.findPlug(canal.c_str(), &status);
		MPlugArray connections;
		if(plug.connectedTo(connections, true, false) && connections.length() > 0 )
			return true;
	}
	return false;
}


/**
 *	Create OSG/OpenGL material corresponding to Maya material
 *
 *	@note Final result will not be exactly the same. Lambert parameters 
 *	are interpreted and converted to OpenGL parameters.
 *	To achieve a more accurate correspondence, we plan to use GLSL shaders
 *	in a future release.
 *
 *	@todo We should make a material cache/register as it is done with textures
 *	to avoid wasting memory pretty badly.
 */
osg::ref_ptr<osg::Material> Shader::material(MObject &obj, bool &mat_trans)
{
	MFnDependencyNode dn(obj);

#ifdef _DEBUG
	std::cout << "Material (" << obj.apiTypeStr() << ") : " << dn.name().asChar() << std::endl;
#endif

	if(obj.hasFn(MFn::kLambert)){

		osg::ref_ptr<osg::Material> material = new osg::Material();
		MFnLambertShader lambert(obj);
		material->setName(lambert.name().asChar());

		// Transparency
		mat_trans = false;
		float opacity;
		if(connectedTexture(obj,"transparency")){
			opacity = 1.0f;
			mat_trans = true;
		}
		else {
			MColor tr = lambert.transparency();
			opacity = 1.0f - ( (tr.r + tr.g + tr.b) / 3.0f ); // average
			if( opacity < 1.0f )
				mat_trans = true;
		}

		// Ambient
		MColor ac = lambert.ambientColor();
		material->setAmbient(osg::Material::FRONT_AND_BACK, 
			osg::Vec4(ac.r, ac.g, ac.b, opacity));

		// Emissive
		MColor ec = lambert.incandescence();
		material->setEmission(osg::Material::FRONT_AND_BACK,
			osg::Vec4(ec.r, ec.g, ec.b, opacity));

		// Diffuse
		MColor color;
		if(connectedTexture(obj,"color"))
			color.r = color.g = color.b = color.a = 1.0;
		else
			color = lambert.color();
		float dc = lambert.diffuseCoeff();
		material->setDiffuse(osg::Material::FRONT_AND_BACK,
			osg::Vec4(color.r*dc, color.g*dc, color.b*dc, opacity));

		if(obj.hasFn(MFn::kReflect)){
			MFnReflectShader reflect(obj);
			// Specular
			MColor spec = reflect.specularColor();
			material->setSpecular(osg::Material::FRONT_AND_BACK,
				osg::Vec4(spec.r, spec.g, spec.b, opacity));
			// Shininess  ***** WRONG! It should take the shininess from other attributes, dependent on the material type  - FIXME!!!
			material->setShininess(osg::Material::FRONT_AND_BACK,
				reflect.reflectivity());
		}

		// Color Mode
		material->setColorMode(osg::Material::OFF);
		return material;
	}

	else {
		std::cerr << "ERROR. Unknown material type (" << 
			obj.apiTypeStr() << ") : " << dn.name().asChar() << std::endl;
	}
	return NULL;
}


/**
 *	Add textures to the StateSet
 *
 *  @param tex_transp Indicates if the material has any texture connected to transparency
 *
 */
void Shader::setTextures(osg::ref_ptr<osg::StateSet> st, MObjectArray &textures, bool tex_transp)
{
	for(int i=0; i<textures.length(); i++){
		// For each texturing unit
		if( textures[i].hasFn(MFn::kFileTexture) ){
			osg::ref_ptr<osg::Texture2D> tex = Texture::exporta(textures[i]);
			if(tex.valid()){

				st->setTextureAttribute(i,tex.get());

// *** NOTA: Se desactiva esto porque provoca que las texturas no aparezcan en los opacos (no entiendo el porqué)
				// Transparencia
/*				if(tex_transp){
					tex->setInternalFormatMode(osg::Texture::USE_IMAGE_DATA_FORMAT);
				}
				else {
					tex->setInternalFormat(GL_RGB);  // NOTA: Ya se establece el InternalFormatMode a USE_USER_DEFINED_FORMAT implícitamente
				}
*/
				// Texture matrix
				osg::ref_ptr<osg::TexMat> texmat = Texture::exportTexMat(textures[i]);
				if(texmat.valid())
					st->setTextureAttribute(i,texmat.get());

				// TexEnv (from configuration)
				osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv();
				texenv->setMode(Config::instance()->getMode());
				st->setTextureAttribute(i,texenv.get());

				st->setTextureMode(i, GL_TEXTURE_2D, osg::StateAttribute::ON);
			}
		}
		else {
			std::cerr << ":-m  Mmmm... I expected a kFileTexture, not a " << textures[0].apiTypeStr() << std::endl;
		}
	}
}


/**
 *
 */
osg::ref_ptr<osg::StateSet> Shader::exporta(MObject &obj, MObjectArray &textures)
{
	MFnDependencyNode dn(obj);
	/*
		Received object is a "shading engine", a set containing the geometry
		to which the shader is applied.
		We must take the connections of this node, and follow them to the shader
		(or shaders) being used.
	*/
	if(obj.hasFn(MFn::kShadingEngine)){
		// Shading engine
#ifdef _DEBUG
		std::cout << "Shading Engine : " << dn.name().asChar() << std::endl;
#endif
		osg::ref_ptr<osg::StateSet> st = new osg::StateSet();

		// We are only interested in the surface shader (other connections are ignored)
		MStatus status;
		MPlug plug = dn.findPlug("surfaceShader", &status);

		MPlugArray connections;
		if(plug.connectedTo(connections, true, false)){
			if(connections.length() > 1){
				std::cerr << "Shader::exporta() There are several connectinos to plug \"surfaceShader\", this can be a problem..." << std::endl;
			}
			if(connections.length() == 1){
                MObject obj = connections[0].node();
				bool mat_trans;		// transparency in the material
				bool tex_trans;		// transparency in the texture
				st->setAttribute( Shader::material(obj, mat_trans).get() );
				tex_trans = connectedTexture(obj,"transparency");
				setTextures( st, textures, tex_trans );
				if( mat_trans || tex_trans ){
					st->setMode(GL_BLEND, osg::StateAttribute::ON);
					st->setAttribute( new osg::BlendFunc( Config::instance()->getBlendFuncSrc(),
														Config::instance()->getBlendFuncDst()) );
					st->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
				}
				return st;
			}
		}
	}
	else {
		std::cerr << "ERROR. Unknown shader type (" << 
			obj.apiTypeStr() << ") : " << dn.name().asChar() << std::endl;
	}
	return NULL;
}
