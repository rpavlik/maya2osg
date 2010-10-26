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

	int numUVSets = meshfn.numUVSets();
	MStringArray namesUVSets;
	std::vector<osg::ref_ptr<osg::Vec2Array> > uv_sets;
	if(numUVSets > 0){
#ifdef _DEBUG
		std::cout << numUVSets << " UV sets" << std::endl;
#endif
		meshfn.getUVSetNames(namesUVSets);
		for(int i=0; i<numUVSets; i++){
//			std::cout << "Unit " << i << " (" << namesUVSets[i].asChar() << ") : " << meshfn.numUVs(namesUVSets[i]) << std::endl;
			if(meshfn.numUVs(namesUVSets[i]) == 0){
				std::cout << " *********** BAD THING IF THE UVSET HAS 0 COORDINATES..." << std::endl;
				//// Remove texture coordinates for this object
				//numUVSets = 0;
				//break;
			}
			MFloatArray us, vs;
			meshfn.getUVs(us, vs, &(namesUVSets[i]));
			osg::ref_ptr<osg::Vec2Array> uv_array = new osg::Vec2Array();
			for(int j=0; j< meshfn.numUVs(namesUVSets[i]); j++){
				uv_array->push_back(osg::Vec2( us[j], vs[j]));
			}
			uv_sets.push_back(uv_array.get());
			// We leave here texture coordinates to apply them when exporting shaders.
			// We don't apply them here because UV sets can be reused for different textures
			// (there is no one-to-one binding)
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
	std::vector< osg::ref_ptr< osg::UIntArray > > UVidx;
	for(int i=0; i<numUVSets; i++)
		UVidx.push_back( new osg::UIntArray() );

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
			normalidx->push_back( itmp.normalIndex(i) );
			// UV sets indices
			for( int j=0; j<numUVSets; j++){
				if(itmp.hasUVs(namesUVSets[j])){
					int iuv;
					itmp.getUVIndex(i, iuv, &(namesUVSets[j]));
					UVidx[j]->push_back(iuv);
				}
				else {
					std::cout << "ERROR. There are polygons without texture coordinates in this mesh." << std::endl;
					UVidx[j]->push_back(0);
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
	geometry->setNormalIndices( normalidx.get() );
//	std::cout << normalidx->size() << " normal indices" << std::endl;
	// Indices of texture coordinates are set when exporting shaders,
	// because UV sets can be reused for different textures (there is no one-to-one binding)

    // Create geode and add the geometry
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(geometry.get());

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
	if(shaders.length() > 0){
		// Check textures bound to this mesh in order to include them in the StateSet
		MObjectArray textures;
		int texture_unit=0;
		for(int i=0; i<numUVSets; i++){
			MObjectArray tex_unit;
			meshfn.getAssociatedUVSetTextures(namesUVSets[i],tex_unit);
			for(int j=0; j<tex_unit.length(); j++){
				// Set the texture coordinates (previously computed)
				geometry->setTexCoordArray(texture_unit, uv_sets[i].get());
				// Set texture coordinates indices (previously computed)
				geometry->setTexCoordIndices(texture_unit, UVidx[i].get());
				// Set the texture for this unit
				textures.append(tex_unit[j]);
				texture_unit++;
			}
//			if(tex_unidad.length() > 1){
//				std::cerr << "Alguien me puede decir qué *@#!% hacen varias texturas asignadas a un sólo UVset ???" << std::endl;
//				// ***** TEXTURAS QUE COMPARTEN COORDENADAS DE MAPEADO. HAY QUE DUPLICAR LAS COORDENADAS EN OPENGL
//			}
//			// nos quedamos con la primera
//			texturas.append(tex_unidad[0]);
		}
		UVidx.clear();

		osg::ref_ptr<osg::StateSet> st = Shader::exporta(shaders[0],textures);
		if(st.valid()) {
			if ( Config::instance()->getUseGLSL() ) {
				ShaderGLSL::exporta(shaders[0], textures, st);
			}
			geode->setStateSet(st.get());
		}
	}

	// Check whether mesh is single or double sided
	// (attributes doubleSided and opposite in surfaceShape node)
	MStatus status;
	MPlug plug = dnodefn.findPlug("doubleSided", &status);
	bool double_sided;
	bool opposite;
	plug.getValue(double_sided);
	plug = dnodefn.findPlug("opposite");
	plug.getValue(opposite);

	osg::StateSet *ss = geode->getOrCreateStateSet();
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
