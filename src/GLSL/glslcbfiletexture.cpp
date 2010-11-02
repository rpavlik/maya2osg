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
#include "glslcbfiletexture.h"
#include "../texture.h"

#include <sstream>
#include <osg/Texture2D>

#include <maya/MFnDependencyNode.h>


/**
 *  Constructor
 */
GLSLCBFileTexture::GLSLCBFileTexture( const MObject &node, osg::StateSet& sset, 
                                      TexturingConfig &texturing_config )
{
    MFnDependencyNode dn(node);
    std::string maya_tex_name = dn.name().asChar();

    _TCSetUnit = texturing_config.getTCSet( maya_tex_name );
    int texture_unit = texturing_config.getTextureUnit( maya_tex_name );

    std::stringstream sampler_name;
    sampler_name << "tex" << texture_unit << "_" << maya_tex_name;
    _samplerName = sampler_name.str();

    // Set the texture in the stateset
	osg::ref_ptr<osg::Texture2D> tex = Texture::exporta(node);
	if(tex.valid()){
		sset.setTextureAttribute(texture_unit, tex.get());
		// Texture matrix
		osg::ref_ptr<osg::TexMat> texmat = Texture::exportTexMat(node);
		if(texmat.valid())
			sset.setTextureAttribute(texture_unit, texmat.get());

//		st.setTextureMode(texture_unit, GL_TEXTURE_2D, osg::StateAttribute::ON);
	}

    // Set the uniform with the texture name
    osg::Uniform *sampler = new osg::Uniform(osg::Uniform::SAMPLER_2D, _samplerName.c_str());
    sampler->setElement(0, texture_unit);
    sset.addUniform( sampler );
}


/// Get the GLSL declarations (constants, global variables, uniforms, etc...)
std::string GLSLCBFileTexture::getDeclarations()
{
    return "uniform sampler2D " + _samplerName + ";";
}


/// Get the GLSL functions needed for this implementation
std::string GLSLCBFileTexture::getFunctions()
{
    return "";
}


/// Get the GLSL inline code
std::string GLSLCBFileTexture::getInlineCode()
{
    std::stringstream shader_src;
    shader_src << "texture2D(" << _samplerName << ", gl_TexCoord[" << _TCSetUnit << "].st)";
    return shader_src.str();
}
