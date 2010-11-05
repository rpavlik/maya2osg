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
#include "mesh.h"
#include "shader.h"
#include "shaderglsl.h"
#include "GLSL/texturingconfig.h"
#include "config.h"
#include "lights.h"
#include "shadows.h"

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/CullFace>
#include <osg/LightModel>

#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MPlug.h>

#include <iostream>

/**
 *  Check wether a Mesh has per vertex colors
 */
bool Mesh::colorPerVertex(MObject &obj)
{
	for( MItMeshPolygon itmp(obj) ; ! itmp.isDone() ; itmp.next() ){
		if(itmp.hasColor())
			return true;
	}
    return false;
}


/**
 *	Create OSG geometry for a Maya Mesh
 *	Returna pointer to an osg::Geode containing an osg::Drawable
 *	of type osg::Geometry with the information about vertices,
 *	primitives and their attributes.
 *
 *  @note This version uses vertex arrays and normal arrays to 
 *	save space
 *
 *  @todo This version exports per-vertex color if color is detected.
 *	We don't consider the case of uniform color, because this case is
 *	assumed to use a material/shader.
 *
 *  SEE: 
 *		MFnMesh::getAssociatedUVSetTextures()
 *		MFnMesh::getConnectedShaders()
 */
osg::ref_ptr<osg::Node> Mesh::exporta(MObject &obj)
{
	MFnMesh meshfn(obj);
	MFnDependencyNode dnodefn(obj);
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

#ifdef _DEBUG
	std::cout << "Exporting Mesh " << dnodefn.name().asChar() <<
		" (" << meshfn.numVertices() << " vertices, " <<
		meshfn.numFaceVertices() << " face vertices" << std::endl;
#endif

    // 1. Build vertex and attributes array

    // -- VERTEX ARRAY
	osg::ref_ptr<osg::Vec3Array> vertex_array = new osg::Vec3Array();
	for(int i=0; i< meshfn.numVertices(); i++){
        MPoint p;
        meshfn.getPoint(i,p);
		vertex_array->push_back( osg::Vec3(p.x, p.y, p.z) );
	}
	geometry->setVertexArray(vertex_array.get());

    if ( Config::instance()->getExportNormals() ) {
        // -- NORMAL ARRAY
	    // NOTE: Is there any way in Maya to know if normals are assigned per-vertex 
	    // or per-face? If there is, we could use other binding modes to reduce
	    // scene size and improve rendering performance.
	    // Meanwhile, we bind normals per vertex to cover every case.
	    osg::ref_ptr<osg::Vec3Array> normal_array = new osg::Vec3Array();
	    MFloatVectorArray normals;	// Per face-vertex normal list
        meshfn.getNormals(normals);
	    for(int i=0; i< normals.length(); i++){
		    normal_array->push_back( osg::Vec3(normals[i].x,
										       normals[i].y,
										       normals[i].z) );
	    }
        geometry->setNormalArray(normal_array.get());
	    geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    }

    // -- COLOR ARRAY (if present)
	bool colors = colorPerVertex(obj);
	osg::ref_ptr<osg::Vec3Array> color_array;
	if(colors){
		color_array = new osg::Vec3Array();
        // Per vertex colors
		MColorArray mcolarray;
		meshfn.getFaceVertexColors(mcolarray);
		for(int i=0; i<mcolarray.length(); i++){
			MColor col = mcolarray[i];
			color_array->push_back( osg::Vec3(col.r, col.g, col.b) );
		}
		geometry->setColorArray(color_array.get());
		geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	}

	// -- TEXTURE COORDINATES (if present)

    // Map of OSG Texture Coordinate Sets (by Maya UV set name)
    TCSetsMap tc_sets_map;

    if ( Config::instance()->getExportTexCoords() ) {
	    int numUVSets = meshfn.numUVSets();
	    MStringArray namesUVSets;
	    if(numUVSets > 0){
#ifdef _DEBUG
		    std::cout << numUVSets << " UV sets" << std::endl;
#endif
		    meshfn.getUVSetNames(namesUVSets);
		    for(int i=0; i<numUVSets; i++){
			    if(meshfn.numUVs(namesUVSets[i]) == 0){
                    std::cerr << "WARNING: UV Set " << namesUVSets[i].asChar() << " has no coordinates. It will be ignored." << std::endl;
				    //// Ignore UV set
                    continue;
			    }
			    MFloatArray us, vs;
			    meshfn.getUVs(us, vs, &(namesUVSets[i]));
			    osg::ref_ptr<osg::Vec2Array> uv_array = new osg::Vec2Array();
			    for(int j=0; j< meshfn.numUVs(namesUVSets[i]); j++){
				    uv_array->push_back(osg::Vec2( us[j], vs[j]));
			    }
                tc_sets_map[namesUVSets[i].asChar()] = uv_array;
                // NOTE: UV sets are accumulated in the map as TC arrays to assign them later to OpenGL texture units
		    }
	    }
    }

	// 2. Build primitives (indices arrays)

    // Primitives counters
	int ntris=0, nquads=0, npolys=0;
    // Vertices indices array
	osg::ref_ptr<osg::UIntArray> vertexidx = new osg::UIntArray();
    // Normals indices array
	osg::ref_ptr<osg::UIntArray> normalidx = new osg::UIntArray();
    // UV indices arrays
    TCSetsIndicesMap tc_idx_map;
    if ( Config::instance()->getExportTexCoords() ) {
        for ( TCSetsMap::iterator i = tc_sets_map.begin() ; i != tc_sets_map.end() ; i++ ) {
            tc_idx_map[i->first] = new osg::UIntArray();
        }
    }

	// Index of indices (face-vertex)
	int ii=0;
    int index_begin_primitive=0;
    // Previous primitive
    int pvc_prev = 0;
    // Traverse all polygons in the mesh
	for( MItMeshPolygon itmp(obj) ; ! itmp.isDone() ; itmp.next() ){

		if( itmp.polygonVertexCount() != pvc_prev ){
			// Primitive change. If there are TRIs or QUADs, we dump them.
			if(ntris>0){
				geometry->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, index_begin_primitive, ntris*3) );
                ntris = 0;
			}
			if(nquads>0){
				geometry->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::QUADS, index_begin_primitive, nquads*4) );
                nquads = 0;
			}
			index_begin_primitive = ii;
		}

		for(int i=0; i<itmp.polygonVertexCount(); i++){

            // vertex index
			vertexidx->push_back( itmp.vertexIndex(i) );
            // normal index
            if ( Config::instance()->getExportNormals() ) {
    			normalidx->push_back( itmp.normalIndex(i) );
            }
            if ( Config::instance()->getExportTexCoords() ) {
			    // UV sets indices
                for( TCSetsMap::iterator tcsi = tc_sets_map.begin() ; tcsi != tc_sets_map.end() ; tcsi++ ) {
                    MString uv_set_name(tcsi->first.c_str());
				    if(itmp.hasUVs(uv_set_name)){
					    int iuv;
					    itmp.getUVIndex(i, iuv, &uv_set_name);
                        tc_idx_map[tcsi->first]->push_back(iuv);
				    }
				    else {
					    std::cerr << "ERROR. There are polygons without texture coordinates in this mesh." << std::endl;
                        tc_idx_map[tcsi->first]->push_back(0);
				    }
			    }
            }

            ii++;
		}

		// End of polygon
		switch(itmp.polygonVertexCount()){
		case 3:
			// TRI
            ntris++;
			break;
		case 4:
			// QUAD
            nquads++;
			break;
		default:
			// POLY
			geometry->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::POLYGON, index_begin_primitive, itmp.polygonVertexCount()) );
            index_begin_primitive = ii;
            npolys++;
            break;
		}
        pvc_prev = itmp.polygonVertexCount();
	}

    if(ntris>0)
		geometry->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, index_begin_primitive, ntris*3) );
    if(nquads>0)
		geometry->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::QUADS, index_begin_primitive, nquads*4) );

    geometry->setVertexIndices( vertexidx.get() );
//	std::cout << vertexidx->size() << " vertex indices" << std::endl;
    if ( Config::instance()->getExportNormals() ) {
    	geometry->setNormalIndices( normalidx.get() );
//	    std::cout << normalidx->size() << " normal indices" << std::endl;
    }
	// Indices of texture coordinates are set when exporting shaders,
	// because UV sets can be reused for different textures (there is no one-to-one binding)

    // Create geode and add the geometry
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(geometry.get());

    // The StateSet of the mesh
    osg::StateSet *ss = geode->getOrCreateStateSet();

	// Check whether mesh is single or double sided
	// (attributes doubleSided and opposite in surfaceShape node)
	MStatus status;
	MPlug plug = dnodefn.findPlug("doubleSided", &status);
	bool double_sided;
	bool opposite;
	plug.getValue(double_sided);
	plug = dnodefn.findPlug("opposite");
	plug.getValue(opposite);

	if ( ( Config::instance()->getSurfaceMode() == Config::KEEP && double_sided ) 
		|| Config::instance()->getSurfaceMode() == Config::DOUBLE ) 
	{
		ss->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );
		osg::ref_ptr<osg::LightModel> lm = Lights::getDefaultLightModel();
		lm->setTwoSided( true );
		ss->setAttribute( lm );
	}
	else {
		ss->setMode( GL_CULL_FACE, osg::StateAttribute::ON );
		osg::CullFace *cull = new osg::CullFace();
		if ( opposite ) {
			cull->setMode(osg::CullFace::FRONT);
		}
		else {
			cull->setMode(osg::CullFace::BACK);
		}
		ss->setAttributeAndModes( cull, osg::StateAttribute::ON );
	}

	// Get the shaders connected to this mesh
	MObjectArray shaders;
	MIntArray indices;
	meshfn.getConnectedShaders(0/*** instanceNumber ???? */, shaders, indices);

	if(shaders.length() > 1){
#ifdef _DEBUG
		for(int i=0; i<shaders.length(); i++){
			MFnDependencyNode dn(shaders[i]);
			std::cout << "Shader " << i << " : " << dn.name().asChar() << std::endl;
		}
#endif
		std::cerr << "WARNING!!! This kMesh has more than one shader applied. OpenGL won't like this at all..." << std::endl;
		std::cerr << "Splitting a kMesh with different shaders applied to different faces is not currently supported" << std::endl;
		// ***** TO-DO : SPLIT KMESHES
	}
	if(shaders.length() > 0){   // Just ONE surface shader

        // Map of connections between textures and UV sets
        TexturingConfig texturing_config;
        if ( Config::instance()->getExportTexCoords() ) {
            // OpenGL texture unit we bind this TC set to
            int texture_unit = 0;
            // For each TC set
            for ( TCSetsMap::iterator i = tc_sets_map.begin() ; i != tc_sets_map.end() ; i++ ) {
                MString uv_set_name( i->first.c_str() );

                // Get textures bound to this UV set in this mesh
			    MObjectArray textures;
			    meshfn.getAssociatedUVSetTextures(uv_set_name, textures);
                // Register the association between textures and UV sets in textures_map 
                // to pass this info to the GLSL shader
                // (in case of fixed function, only the first TC set will be used)
                for( unsigned int j = 0 ; j < textures.length() ; j++ ) {
                    MFnDependencyNode texture(textures[j]);
                    // Store the association of each texture to the TC set (OpenGL texture unit)
                    texturing_config.setTCSet( texture.name().asChar(), texture_unit );
                }

                // Set the texture coordinates (previously computed)
			    geometry->setTexCoordArray(texture_unit, i->second);
			    // Set texture coordinates indices (previously computed)
			    geometry->setTexCoordIndices(texture_unit, tc_idx_map[i->first]);
                texture_unit++;
            }
        }
        if ( Config::instance()->getUseGLSL() ) {
            ShaderGLSL::exporta(shaders[0], texturing_config, *ss);
        }
        else {
            // Configure the StateSet for fixed function OpenGL pipeline
		    Shader::exporta(shaders[0], *ss);
        }
	}

	// Shadows
	if ( Config::instance()->getComputeShadows() ) {
		if ( ! dnodefn.findPlug("castsShadows").asBool() ) {
			geode->setNodeMask( geode->getNodeMask() & ~Shadows::CastsShadowTraversalMask );
		}
		if ( ! dnodefn.findPlug("receiveShadows").asBool() ) {
			geode->setNodeMask( geode->getNodeMask() & ~Shadows::ReceivesShadowTraversalMask );
		}
	}

    // *** FIXME!!!
//    geometry->setUseDisplayList( false );

    // Name the node (mesh)
	geode->setName( dnodefn.name().asChar() );

	return (osg::Node *)geode.get();
}
