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
#include "filetexture.h"
#include "shadingnetwork.h"
#include "bump2normal.h"
#include "../texture.h"
#include "../shader.h"
#include "../config.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>

#include <osgDB/FileNameUtils>

#include <sstream>


/**
 *  Constructor
 */
FileTexture::FileTexture( const MObject &shading_node, ShadingNetwork &shading_network ) :
    ShadingNode( shading_node, shading_network )
{
}


/**
 *  Get the GLSL Codeblock for a plug
 */
ShadingNode::CodeBlock FileTexture::getCodeBlock( const std::string &plug_name )
{
    CodeBlock code_block;

    std::string variable_name;
    MFnDependencyNode dn(_mayaShadingNode);
    std::string maya_tex_name = dn.name().asChar();

    // Check the plug name. Supported ones: outColor, outTransparency, outAlpha

    if ( plug_name == "outColor" ||
         plug_name == "outTransparency" ||
         plug_name == "outAlpha" )
    {
        variable_name = "sn_filetexture_" + maya_tex_name + "_output";

        MPlugArray remote_plugs;
        Shader::getPlugConnectedFromChannel(_mayaShadingNode, plug_name, remote_plugs);
        bool is_bump = false;
		if ( Config::instance()->getEnableBumpMapping() ) {
			for ( int i=0 ; !is_bump && i < remote_plugs.length() ; i++ ) {
				std::string remote_plug_name = remote_plugs[i].partialName(false, false, false, false, false, true).asChar();
				bool has_Fn_kBump = remote_plugs[i].node().hasFn(MFn::kBump);
				if ( remote_plug_name == "bumpValue" && has_Fn_kBump )
				{
					is_bump = true;
					break;
				}
			}
		}
        std::string filename_nm;
        if ( is_bump ) {
	        MPlug plug_filename;
	        MString texname;
	        plug_filename = dn.findPlug("fileTextureName");
	        plug_filename.getValue(texname);
            // This texture is used as a bump (height) map, we should convert it to a normal map
            std::string filename_orig = texname.asChar();

            // Convert bump map (height map) to normal map
            filename_nm = bump2Normal( filename_orig );
        }

        // If variable not available, we compute them
        if ( ! variableIsAvailable(variable_name) ) {

            int tc_set_unit = _shadingNetwork.getTexturingConfig().getTCSet( maya_tex_name );
            int texture_unit = _shadingNetwork.getTexturingConfig().getTextureUnit( maya_tex_name );

            std::stringstream ss_sampler_name;
            ss_sampler_name << "tex" << texture_unit << "_" << maya_tex_name;
            std::string sampler_name = ss_sampler_name.str();

            // Set the texture in the stateset
	        osg::ref_ptr<osg::Texture2D> tex;
            if ( is_bump )
                tex = Texture::exporta(_mayaShadingNode, filename_nm);
            else
                tex = Texture::exporta(_mayaShadingNode);

            if(tex.valid()){
		        _shadingNetwork.getStateSet().setTextureAttribute(texture_unit, tex.get());
		        // Texture matrix
		        osg::ref_ptr<osg::TexMat> texmat = Texture::exportTexMat(_mayaShadingNode);
		        if(texmat.valid())
			        _shadingNetwork.getStateSet().setTextureAttribute(texture_unit, texmat.get());

//        		st.setTextureMode(texture_unit, GL_TEXTURE_2D, osg::StateAttribute::ON);
	        }

            // Set the uniform with the texture name
            osg::Uniform *sampler = new osg::Uniform(osg::Uniform::SAMPLER_2D, sampler_name);
            sampler->setElement(0, texture_unit);
            _shadingNetwork.getStateSet().addUniform( sampler );

            code_block.declarations = "uniform sampler2D " + sampler_name + ";\n";

            // No functions

            std::stringstream shader_src;
            shader_src << "    vec4 " << variable_name << " = texture2D(" << sampler_name << ", gl_TexCoord[" << tc_set_unit << "].st);\n";
            code_block.computeCode = shader_src.str();

            _computedVariables.insert(variable_name);
        }

        // Access code
        if ( is_bump ) {
            // If it is a bump map, it has been converted to an RGB normal map, so the output is .rgb
            code_block.accessCode = variable_name + ".rgb";
        }
        else if ( plug_name == "outColor" ) {
            code_block.accessCode = variable_name + ".rgb";
        }
        else if ( plug_name == "outAlpha" ) {
            code_block.accessCode = variable_name + ".a";
        }
        else if ( plug_name == "outTransparency" ) {
            code_block.accessCode = "(1.0 - " + variable_name + ".a)";
        }
    }
    else {
        std::cerr << "WARNING: Unsupported plug " << plug_name << " in FileTexture shading node " << dn.name().asChar() << std::endl;
        // throw exception ??? !!! FIXME!!!
    }

    return code_block;
}
