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
#include "common.h"

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
 *	Check whether there is any node connected to any channel of this material (color, transparency, ...)
 */
bool Shader::connectedChannel(const MObject &shading_node, std::string channel)
{
	MFnDependencyNode dn(shading_node);
	MStatus status;
	MPlug plug = dn.findPlug(channel.c_str(), true, &status);
    if ( MCheckStatus(status, "ERROR: Could not find plug " + channel ) ) {
        return false;
    }
	MPlugArray connections;
    if(plug.connectedTo(connections, true, false) && connections.length() > 0 ) {
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
osg::ref_ptr<osg::Material> Shader::material(const MObject &surface_shader, bool &mat_trans)
{
	MFnDependencyNode dn(surface_shader);

#ifdef _DEBUG
	std::cout << "Material (" << surface_shader.apiTypeStr() << ") : " << dn.name().asChar() << std::endl;
#endif

	if(surface_shader.hasFn(MFn::kLambert)){

		osg::ref_ptr<osg::Material> material = new osg::Material();
		MFnLambertShader lambert(surface_shader);
		material->setName(lambert.name().asChar());

		// Transparency
		mat_trans = false;
		float opacity;
		if(connectedChannel(surface_shader,"transparency")){
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
		if(connectedChannel(surface_shader,"color"))
			color.r = color.g = color.b = color.a = 1.0;
		else
			color = lambert.color();
		float dc = lambert.diffuseCoeff();
		material->setDiffuse(osg::Material::FRONT_AND_BACK,
			osg::Vec4(color.r*dc, color.g*dc, color.b*dc, opacity));

		if(surface_shader.hasFn(MFn::kReflect)){
			MFnReflectShader reflect(surface_shader);
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
			surface_shader.apiTypeStr() << ") : " << dn.name().asChar() << std::endl;
	}
	return NULL;
}


/**
 *	@param obj ShadingEngine (ShadingGroup) object
 */
void Shader::exporta(const MObject &shading_engine, osg::StateSet &state_set)
{
	MFnDependencyNode dn(shading_engine);
	/*
		Received object is a "shading engine", a set containing the geometry
		to which the shader is applied.
		We must take the connections of this node, and follow them to the shader
		(or shaders) being used.
	*/
	if(shading_engine.hasFn(MFn::kShadingEngine)){
		// Shading engine
#ifdef _DEBUG
		std::cout << "Shading Engine : " << dn.name().asChar() << std::endl;
#endif

		// We are only interested in the surface shader (other connections are ignored)
		MStatus status;
		MPlug plug = dn.findPlug("surfaceShader", &status);

		MPlugArray connections;
		if(plug.connectedTo(connections, true, false)){
			if(connections.length() > 1){
				std::cerr << "Shader::exporta() There are several connections to plug \"surfaceShader\", this can be a problem..." << std::endl;
			}
			if(connections.length() == 1){
                MObject surface_shader = connections[0].node();
				bool mat_trans = false;		// transparency in the material
				bool tex_trans = false;		// transparency in the texture
				state_set.setAttribute( Shader::material(surface_shader, mat_trans).get() );
				tex_trans = connectedChannel(surface_shader,"transparency");
                setColorTexture( state_set, surface_shader );
				if( mat_trans || tex_trans ){
					state_set.setMode(GL_BLEND, osg::StateAttribute::ON);
					state_set.setAttribute( new osg::BlendFunc( Config::instance()->getBlendFuncSrc(),
														Config::instance()->getBlendFuncDst()) );
					state_set.setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
				}
			}
		}
	}
	else {
		std::cerr << "ERROR. Unknown shading engine (" << 
			shading_engine.apiTypeStr() << ") : " << dn.name().asChar() << std::endl;
	}
}


/**
 *	Get the surface shader applied to the object
 *
 *	@warning Only the first shading engine is returned
 */
void Shader::getShadingEngine(const MObject &dependency_node, MObject &shading_engine)
{
	MFnDependencyNode dn(dependency_node);

	// Get output connections to locate the shading engine
	MPlugArray conns;
	dn.getConnections(conns);
	for(int i=0; i<conns.length(); i++){
		MPlug conn = conns[i];
		MPlugArray connectedTo;
		// Get the connections having this node as source
		conn.connectedTo(connectedTo, false, true);
		for(int j=0; j<connectedTo.length(); j++){
			MPlug destination = connectedTo[j];
			MObject destination_node = destination.node();
			if(destination_node.hasFn(MFn::kShadingEngine)){
				MFnDependencyNode dn_shading_engine(destination_node);
				std::cout << "SHADING ENGINE " << dn_shading_engine.name().asChar() << std::endl;
				shading_engine = destination_node;
				return;
			}
		}
	}
	std::cerr << "WARNING: No ShadingEngine connected to node " << dn.name() << std::endl;
}


/**
 *	Get the surface shader applied to the shading engine
 */
void Shader::getSurfaceShader(const MObject &shading_engine, MObject &surface_shader)
{
	MFnDependencyNode dn(shading_engine);
	MPlug plug_surface_shader = dn.findPlug("surfaceShader", true);
	MPlugArray connectedTo;
	// Get the connections having this node as destination
	plug_surface_shader.connectedTo(connectedTo, true, false);
	for(int j=0; j<connectedTo.length(); j++){
		MPlug source = connectedTo[j];
		MObject source_node = source.node();
		MFnDependencyNode dn_surface_shader(source_node);
//		std::cout << "SURFACE SHADER : " << dn_surface_shader.name().asChar() << std::endl;
		surface_shader = source_node;
	}
}


/**
 *  Get the plug connected to a channel of a shading node
 */
void Shader::getPlugConnectedToChannel( const MObject &shading_node, std::string channel, MPlug &remote_plug )
{
	MFnDependencyNode dn(shading_node);
	MPlug plug = dn.findPlug(channel.c_str(), true);
	MPlugArray connectedTo;
	// Get the connections having this node as destination
	plug.connectedTo(connectedTo, true, false);
    // We consider only the first (should be the only one) connection
    if ( connectedTo.length() > 0 ) {
        remote_plug = connectedTo[0];
    }
}


/**
 *  Get the plug connected from a channel of a shading node
 */
void Shader::getPlugConnectedFromChannel( const MObject &shading_node, std::string channel, MPlugArray &remote_plugs )
{
	MFnDependencyNode dn(shading_node);
	MPlug plug = dn.findPlug(channel.c_str(), true);
	// Get the connections having this node as destination
	plug.connectedTo(remote_plugs, false, true);
}


/**
 *  Get the node connected to a channel of the surface shader
 */
void Shader::getNodeConnectedToChannel( const MObject &shading_node, std::string channel, MObject &node )
{
    MPlug remote_plug;
    getPlugConnectedToChannel( shading_node, channel, remote_plug );
    node = remote_plug.node();
}


/**
 *	Get the file texture connected to color channel (if any)
 */
void Shader::getColorTexture( const MObject &surface_shader, MObject &texture )
{
    getNodeConnectedToChannel( surface_shader, "color", texture );
}


/**
 *  Establish the color (and maybe transparency) texture
 *
 *  @return Color texture contains transparency/opacity/alpha channel
 */
bool Shader::setColorTexture(osg::StateSet &st, const MObject &surface_shader, int texture_unit)
{
	MFnDependencyNode dn(surface_shader);
    bool transparent;

    MObject color_texture;
    MPlug plug_color = dn.findPlug("color");
    if ( plug_color.isConnected() ) {
        // Get the color texture
        MPlugArray connected_to_color;
        plug_color.connectedTo( connected_to_color, true, false );
        // NOTE: We assume that only one node is connected to color channel
        color_texture = connected_to_color[0].node();
		if( color_texture.hasFn(MFn::kFileTexture) ){
			osg::ref_ptr<osg::Texture2D> tex = Texture::exporta(color_texture);
			if(tex.valid()){
				st.setTextureAttribute(texture_unit, tex.get());
				// Texture matrix
				osg::ref_ptr<osg::TexMat> texmat = Texture::exportTexMat(color_texture);
				if(texmat.valid())
					st.setTextureAttribute(texture_unit, texmat.get());

				// TexEnv (from configuration)
				osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv();
				texenv->setMode(Config::instance()->getMode());
				st.setTextureAttribute(texture_unit,texenv.get());

				st.setTextureMode(texture_unit, GL_TEXTURE_2D, osg::StateAttribute::ON);
			}
		}
		else {
			std::cerr << ":-m  Mmmm... I expected a kFileTexture, not a " << color_texture.apiTypeStr() << std::endl;
		}
    }
    MPlug plug_transparency = dn.findPlug("transparency");
    if ( plug_transparency.isNetworked() ) {
#if 1
        // Get the transparency texture
        MPlugArray connected_to_transparency;
        plug_color.connectedTo( connected_to_transparency, true, false );
        // NOTE: We assume that only one node is connected to transparency channel
        MObject transparency_texture = connected_to_transparency[0].node();
        if ( transparency_texture != color_texture ) {
            std::cerr << "WARNING: Transparency texture is different from color texture" << std::endl;
        }
#endif
        transparent = true;
    }
/*    else {
        transparent = dn.findPlug("transparencyR").asFloat() > 0.0
            || dn.findPlug("transparencyG").asFloat() > 0.0
            || dn.findPlug("transparencyB").asFloat() > 0.0;
    }*/
    return transparent;
}
