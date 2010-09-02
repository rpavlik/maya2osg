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
#include "importervisitor.h"

#include <iostream>
#include <sstream>
#include <algorithm>

#include <osg/Group>
#include <osg/Transform>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Array>

#include <maya/MStatus.h>
#include <maya/MFnTransform.h>
#include <maya/MMatrix.h>
#include <maya/MFnMesh.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>

#include "proxymaterials.h"

#define CHECK_POINTER(a)	if(a==NULL) std::cerr << "ERROR. Pointer " << #a << " is null" << std::endl;

/**
 *	Name Maya node, trying to keep the OSG name
 *
 *	@warning Maya does not accept some characters for the names
 *	that can be present in OSG node names. We need to convert
 *	these problematic names.
 *
 *	@note If there is no node name, we do not establish any,
 *	because Maya will automagically name it
 */
void nameNode(osg::Node &OSG_node, MObject &obj)
{
	std::string name = OSG_node.getName();
//	std::cout << "Rewriting name " << name << std::endl;
	if ( name.length() > 0 ){
		// Remove bars and dots (special characters used in Maya as 
		// separators for path nodes and attribute names respectively)
		replace(name.begin(), name.end(), '|', '_');
		replace(name.begin(), name.end(), '.', '_');
		// Maya does not like spaces and ':' either...
		replace(name.begin(), name.end(), ' ', '_');
		replace(name.begin(), name.end(), ':', '_');
		MFnDependencyNode dnodefn(obj);
		dnodefn.setName(MString( name.c_str() ));
	}
}

/**
 *
 */
void ImporterVisitor::apply (osg::Group &node)
{
//	std::cout << "Importing osg::Group" << std::endl;

	// Create Maya object
	MFnTransform mfn_trans;
	MObject transform = mfn_trans.create(_parent);

	// Establish current parent, traverse the children and recover previous parent
	MObject previous_parent = _parent;
	_parent = transform;
	node.traverse(*this);
	_parent = previous_parent;

	// Establish node name if there is any in OSG (if there is no name, 
	// Maya will assign a default name)
	nameNode(node, mfn_trans.object());
}

/**
 *
 *	@todo Check whether the order of matrix elements is correct
 */
void ImporterVisitor::apply (osg::Transform &node)
{
//	std::cout << "Importing osg::Transform" << std::endl;

	// Create Maya object
	MFnTransform mfn_trans;
	MObject transform = mfn_trans.create(_parent);

	// Get OSG transform matrix and pass it to the MObject
	osg::Matrix osg_matrix = node.asMatrixTransform()->getMatrix();
	double matrix[4][4];
	for( int i=0; i<4; i++)
		for( int j=0; j<4; j++) {
			// ***** WARNING: Is the matrix elements order the same for Maya and OpenGL?
			matrix[i][j] = osg_matrix.ptr()[i*4+j];
		}
	mfn_trans.set(MTransformationMatrix( MMatrix(matrix) ) );

	// Establish current parent, traverse the children and recover previous parent
	MObject previous_parent = _parent;
	_parent = transform;
	node.traverse(*this);
	_parent = previous_parent;

	// Establish node name if there is any in OSG (if there is no name, 
	// Maya will assign a default name)
	nameNode(node, mfn_trans.object());
}

/**
 *
 */
void ImporterVisitor::apply (osg::Geode &node)
{
//	std::cout << "Importing osg::Geode " << node.getName() << std::endl;

	// Create Maya object
	MFnTransform mfn_trans;
	MObject transform = mfn_trans.create(_parent);

	// Establish current parent, traverse the children and recover previous parent
	MObject previous_parent = _parent;
	_parent = transform;
	// Import independently every drawable in this Geode
	for(unsigned int i=0; i<node.getNumDrawables() ; i++){
		importDrawable(node.getDrawable(i));
	}
	_parent = previous_parent;

	// Establish node name if there is any in OSG (if there is no name, 
	// Maya will assign a default name)
	nameNode(node,mfn_trans.object());
}

/**
 *
 *	@todo Import the stateset (material, textures, ...)
 */
void ImporterVisitor::importDrawable(osg::Drawable *drawable)
{
//	std::cout << "Importing osg::Drawable" << std::endl;
	
	MObject shading_engine;
	createShadingEngine( drawable->getOrCreateStateSet(), shading_engine );

	if( drawable->asGeometry() != NULL ){
		importGeometry(drawable->asGeometry(), shading_engine);
	}
	else {
/*		osg::ShapeDrawable *shape_drawable = dynamic_cast<osg::ShapeDrawable *>(dibujable);
		if ( shape_drawable != NULL){
			importShapeDrawable(shape_drawable, shading_engine);
		}
		else {*/
			std::cerr << "WARNING: Drawable not supported by the importer." << std::endl;
//		}
	}
}

/**
 *
 *	@todo Under construction...
 */
void ImporterVisitor::importGeometry(osg::Geometry *geometry, MObject &shading_engine)
{
//	std::cout << "Importing osg::Geometry" << std::endl;

	MStatus err;	// Error code
#define ERROR_CHECK(msg) if ( err != MS::kSuccess ) std::cerr << "ERROR: " << msg << std::endl;

	// Vertices
	osg::Vec3Array *osg_varray = dynamic_cast<osg::Vec3Array *>(geometry->getVertexArray());
	CHECK_POINTER(osg_varray);
	int num_vertices = osg_varray->getNumElements();
//	std::cout << "Geometry with " << num_vertices << " vertices" << std::endl;
	MFloatPointArray vertex_array;
	for(int i=0; i<num_vertices; i++){
		vertex_array.append((*osg_varray)[i].x(),
							(*osg_varray)[i].y(),
							(*osg_varray)[i].z());
	}

	// Indices
	osg::IndexArray *osg_iarray = dynamic_cast<osg::IndexArray *>(geometry->getVertexIndices());
	int num_indices = 0;
	if(osg_iarray!=NULL)
		num_indices = osg_iarray->getNumElements();
//	std::cout << "Geometry with " << num_indices << " indices" << std::endl;

	// Normals
/*	osg::Vec3Array *osg_narray = dynamic_cast<osg::Vec3Array *>(geometry->getNormalArray());
	int num_normals = osg_narray->getNumElements();
	std::cout << "Geometry with " << num_normals << " normals" << std::endl;
	MVectorArray normal_array;
	MIntArray normal_indices;
	for(int i=0; i<num_normals; i++){
//		std::cout << "Converting OSG normal " << i << " : " << (*osg_narray)[i] << std::endl;
		MVector vector((*osg_narray)[i].x(),
						(*osg_narray)[i].y(),
						(*osg_narray)[i].z());
//		std::cout << " to Maya normal " << vector << std::endl;
		vector.normalize();	// ***** PARANOIA!
		normal_array.append(vector);
	}
/*	int vertex_normal_index = 0;
	int face_normal_index = 0;
	MIntArray pervertex_normal_indices;	// Per-vertex normal indices (for setFaceVertexNormals() )
	MIntArray perface_normal_indices;		// Per-face normal indices (for setFaceVertexNormals() )*/
//	switch( geometry->getNormalBinding() ){
/*		case osg::Geometry::BIND_OFF:
			num_normals = 0;
			break;
		case osg::Geometry::BIND_OVERALL:
			for( int i=0; i<num_vertices; i++){
				normal_indices.append(0);
			}
			break;
		case osg::Geometry::BIND_PER_PRIMITIVE_SET:
			std::cout << "### BIND PER PRIMITIVE SET" << std::endl;
			for(unsigned int i=0; i<geometry->getNumPrimitiveSets(); i++){
				osg::PrimitiveSet *ps = geometry->getPrimitiveSet(i);
				switch(ps->getMode()){
					case osg::PrimitiveSet::TRIANGLES:
						break;
					case osg::PrimitiveSet::TRIANGLE_STRIP:
						break;
					case osg::PrimitiveSet::TRIANGLE_FAN:
						break;
					case osg::PrimitiveSet::QUADS:
						for(unsigned int j=0; j<ps->getNumIndices(); j++)
							pervertex_normal_indices.append(vertex_normal_index++);
						for(unsigned int j=0; j<ps->getNumIndices()/4; j++)
							perface_normal_indices.append(face_normal_index++);
						break;
					case osg::PrimitiveSet::QUAD_STRIP:
						break;
					case osg::PrimitiveSet::POLYGON:
						break;
					default:
						std::cerr << "Venga ya, hombre! ... qué coño de normales le vas a poner a una primitiva " << ps->getMode() << std::endl;
						break;
				}
			}
			break;
		case osg::Geometry::BIND_PER_PRIMITIVE:
			std::cout << "### BIND PER PRIMITIVE" << std::endl;
			for(unsigned int i=0; i<geometry->getNumPrimitiveSets(); i++){
				osg::PrimitiveSet *ps = geometry->getPrimitiveSet(i);
				std::cout << ps->getNumIndices() << " índice(s) y " << ps->getNumPrimitives() << " primitiva(s)" << std::endl;
				switch(ps->getMode()){
					case osg::PrimitiveSet::TRIANGLES:
						break;
					case osg::PrimitiveSet::TRIANGLE_STRIP:
						break;
					case osg::PrimitiveSet::TRIANGLE_FAN:
						break;
					case osg::PrimitiveSet::QUADS:
						for(unsigned int j=0; j<ps->getNumIndices(); j++)
							pervertex_normal_indices.append(vertex_normal_index++);
						for(unsigned int j=0; j<ps->getNumIndices()/4; j++)
							perface_normal_indices.append(face_normal_index++);
						break;
					case osg::PrimitiveSet::QUAD_STRIP:
						break;
					case osg::PrimitiveSet::POLYGON:
						break;
					default:
						std::cerr << "Venga ya, hombre! ... qué coño de normales le vas a poner a una primitiva " << ps->getMode() << std::endl;
						break;
				}
			}
			break;*/
/*		case osg::Geometry::BIND_PER_VERTEX:
			for( int i=0; i<num_vertices; i++){
				normal_indices.append(i);
			}
			break;
		default:
			std::cout << "################## NPI ##################" << std::endl;
	}*/

	// Polygons
	int num_polygons = 0;
	MIntArray polygon_counts;
	MIntArray polygon_connects;

// If there are indices in the drawable, we take these indices. If there are not, it is sequential
#define INDEX(j)	(num_indices>0)?(osg_iarray->index(da->index(j))):(da->index(j))

	// Processs each PrimitiveSet
	for(unsigned int i=0; i<geometry->getNumPrimitiveSets(); i++){
//		std::cout << "Primitive Set " << i+1 << " of " << geometry->getNumPrimitiveSets() << std::endl;
		bool ccw = true;
		osg::PrimitiveSet *ps = geometry->getPrimitiveSet(i);
		//#######################################################
/*		switch(ps->getMode()){
			case osg::PrimitiveSet::POINTS:
			case osg::PrimitiveSet::LINES:
			case osg::PrimitiveSet::LINE_STRIP:
			case osg::PrimitiveSet::LINE_LOOP:
				break;
			case osg::PrimitiveSet::TRIANGLES:
				std::cout << "TRIANGLES" << std::endl;
				break;
			case osg::PrimitiveSet::TRIANGLE_STRIP:
				std::cout << "TRIANGLE_STRIP" << std::endl;
				break;
			case osg::PrimitiveSet::TRIANGLE_FAN:
				std::cout << "TRIANGLE_FAN" << std::endl;
				break;
			case osg::PrimitiveSet::QUADS:
				std::cout << "QUADS" << std::endl;
				break;
			case osg::PrimitiveSet::QUAD_STRIP:
				std::cout << "QUAD_STRIP" << std::endl;
				break;
			case osg::PrimitiveSet::POLYGON:
				std::cout << "POLYGON" << std::endl;
				break;
		}*/
		//#######################################################
		switch(ps->getType()){
			case osg::PrimitiveSet::DrawArraysPrimitiveType:
//				std::cout << "DrawArrays" << std::endl;
				{
					osg::DrawArrays *da = dynamic_cast<osg::DrawArrays *>(ps);
					CHECK_POINTER(da)
					switch(ps->getMode()){
						case osg::PrimitiveSet::POINTS:
						case osg::PrimitiveSet::LINES:
						case osg::PrimitiveSet::LINE_STRIP:
						case osg::PrimitiveSet::LINE_LOOP:
							std::cerr << "Primitive not supported : " << ps->getMode() << std::endl;
							break;
						case osg::PrimitiveSet::TRIANGLES:
							for(unsigned int j=0; j<ps->getNumIndices(); ){
								polygon_counts.append(3);
								polygon_connects.append(INDEX(j++));
								polygon_connects.append(INDEX(j++));
								polygon_connects.append(INDEX(j++));
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::TRIANGLE_STRIP:
							for(unsigned int j=0; j<ps->getNumIndices()-2; ){
								polygon_counts.append(3);
								if(ccw){
									polygon_connects.append(INDEX(j));
									polygon_connects.append(INDEX(j+1));
									polygon_connects.append(INDEX(j+2));
									ccw = false;
								} else {
									polygon_connects.append(INDEX(j+2));
									polygon_connects.append(INDEX(j+1));
									polygon_connects.append(INDEX(j));
									ccw = true;
								}
								j++;
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::TRIANGLE_FAN:
							for(unsigned int j=0; j<ps->getNumIndices()-2; ){
								polygon_counts.append(3);
								polygon_connects.append(INDEX(0));
								polygon_connects.append(INDEX(j+1));
								polygon_connects.append(INDEX(j+2));
								j++;
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::QUADS:
							for(unsigned int j=0; j<ps->getNumIndices(); ){
								polygon_counts.append(4);
								polygon_connects.append(INDEX(j++));
								polygon_connects.append(INDEX(j++));
								polygon_connects.append(INDEX(j++));
								polygon_connects.append(INDEX(j++));
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::QUAD_STRIP:
							for(unsigned int j=0; j<ps->getNumIndices()-3; ){
								polygon_counts.append(4);
								if(ccw){
									polygon_connects.append(INDEX(j));
									polygon_connects.append(INDEX(j+1));
									polygon_connects.append(INDEX(j+2));
									polygon_connects.append(INDEX(j+3));
									ccw = false;
								} else {
									polygon_connects.append(INDEX(j+3));
									polygon_connects.append(INDEX(j+2));
									polygon_connects.append(INDEX(j+1));
									polygon_connects.append(INDEX(j));
									ccw = true;
								}
								j+=2;
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::POLYGON:
							polygon_counts.append(ps->getNumIndices());
							num_polygons++;
							for(unsigned int j=0; j<ps->getNumIndices(); j++){
								polygon_connects.append(INDEX(j));
							}
							break;
					}
				}
				break;
			case osg::PrimitiveSet::DrawArrayLengthsPrimitiveType:
//				std::cout << "DrawArrayLengths" << std::endl;
				{
					osg::DrawArrayLengths *da = dynamic_cast<osg::DrawArrayLengths *>(ps);
					CHECK_POINTER(da)
					int j = 0;
					switch(ps->getMode()){
						case osg::PrimitiveSet::POINTS:
						case osg::PrimitiveSet::LINES:
						case osg::PrimitiveSet::LINE_STRIP:
						case osg::PrimitiveSet::LINE_LOOP:
							std::cerr << "Primitive not supported : " << ps->getMode() << std::endl;
							break;
						case osg::PrimitiveSet::TRIANGLES:
							for(osg::DrawArrayLengths::iterator it=da->begin(); it!=da->end(); it++){
								for(int k=0; k<((*it)/3)+1; k++){
									polygon_counts.append(3);
									polygon_connects.append(INDEX(j++));
									polygon_connects.append(INDEX(j++));
									polygon_connects.append(INDEX(j++));
									num_polygons++;
								}
							}
						case osg::PrimitiveSet::TRIANGLE_STRIP:
							for(osg::DrawArrayLengths::iterator it=da->begin(); it!=da->end(); it++){
								for(int k=0; k<(*it)-2; k++){
									polygon_counts.append(3);
									if(ccw){
										polygon_connects.append(INDEX(j));
										polygon_connects.append(INDEX(j+1));
										polygon_connects.append(INDEX(j+2));
										ccw = false;
									} else {
										polygon_connects.append(INDEX(j+2));
										polygon_connects.append(INDEX(j+1));
										polygon_connects.append(INDEX(j));
										ccw = true;
									}
									j++;
									num_polygons++;
								}
								j+=2;
							}
							break;
						case osg::PrimitiveSet::TRIANGLE_FAN:
							for(osg::DrawArrayLengths::iterator it=da->begin(); it!=da->end(); it++){
								for(int k=0; k<(*it)-2; k++){
									polygon_counts.append(3);
									polygon_connects.append(INDEX(0));
									polygon_connects.append(INDEX(j+1));
									polygon_connects.append(INDEX(j+2));
									j++;
									num_polygons++;
								}
								j+=2;
							}
							break;
						case osg::PrimitiveSet::QUADS:
							for(osg::DrawArrayLengths::iterator it=da->begin(); it!=da->end(); it++){
								for(int k=0 ; k < (*it)/4 ; k++){
									polygon_counts.append(4);
									polygon_connects.append(INDEX(j++));
									polygon_connects.append(INDEX(j++));
									polygon_connects.append(INDEX(j++));
									polygon_connects.append(INDEX(j++));
									num_polygons++;
								}
							}
							break;
						case osg::PrimitiveSet::QUAD_STRIP:
							for(osg::DrawArrayLengths::iterator it=da->begin(); it!=da->end(); it++){
								for(int k=0; k<((*it)/2)-1; k++){
									polygon_counts.append(4);
									if(ccw){
										polygon_connects.append(INDEX(j));
										polygon_connects.append(INDEX(j+1));
										polygon_connects.append(INDEX(j+2));
										polygon_connects.append(INDEX(j+3));
										ccw = false;
									} else {
										polygon_connects.append(INDEX(j+3));
										polygon_connects.append(INDEX(j+2));
										polygon_connects.append(INDEX(j+1));
										polygon_connects.append(INDEX(j));
										ccw = true;
									}
									j+=2;
									num_polygons++;
								}
								j+=2;
							}
							break;
						case osg::PrimitiveSet::POLYGON:
							for(osg::DrawArrayLengths::iterator it=da->begin(); it!=da->end(); it++){
								polygon_counts.append(*it);
								num_polygons++;
								for(int k=0; k<(*it); k++){
									polygon_connects.append(INDEX(j++));
								}
							}
							break;
					}
				}
				break;
			case osg::PrimitiveSet::DrawElementsUBytePrimitiveType:
//				std::cout << "DrawElementsUByte" << std::endl;
				{
					osg::DrawElementsUByte *de = dynamic_cast<osg::DrawElementsUByte *>(ps);
					CHECK_POINTER(de)
					switch(ps->getMode()){
						case osg::PrimitiveSet::POINTS:
						case osg::PrimitiveSet::LINES:
						case osg::PrimitiveSet::LINE_STRIP:
						case osg::PrimitiveSet::LINE_LOOP:
							std::cerr << "Primitive not supported : " << ps->getMode() << std::endl;
							break;
						case osg::PrimitiveSet::TRIANGLES:
							for( osg::DrawElementsUByte::iterator it=de->begin(); it!=de->end(); ){
								polygon_counts.append(3);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::TRIANGLE_STRIP:
							for( unsigned int j = 0; j < de->getNumIndices() - 2; j++ ){
								polygon_counts.append(3);
								if(ccw){
									polygon_connects.append(de->index(j));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j+2));
									ccw = false;
								} else {
									polygon_connects.append(de->index(j+2));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j));
									ccw = true;
								}
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::TRIANGLE_FAN:
							for( unsigned int j = 0; j < de->getNumIndices() - 2; j++ ){
								polygon_counts.append(3);
								polygon_connects.append(de->index(0));
								polygon_connects.append(de->index(j+1));
								polygon_connects.append(de->index(j+2));
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::QUADS:
							for( osg::DrawElementsUByte::iterator it=de->begin(); it!=de->end(); ){
								polygon_counts.append(4);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::QUAD_STRIP:
							for( unsigned int j = 0; j < (de->getNumIndices()/2)-1; j+=2){
								polygon_counts.append(4);
								if(ccw){
									polygon_connects.append(de->index(j));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j+2));
									polygon_connects.append(de->index(j+3));
									ccw = false;
								} else {
									polygon_connects.append(de->index(j+3));
									polygon_connects.append(de->index(j+2));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j));
									ccw = true;
								}
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::POLYGON:
							polygon_counts.append(de->getNumIndices());
							num_polygons++;
							for( osg::DrawElementsUByte::iterator it=de->begin(); it!=de->end(); ){
								polygon_connects.append(*it++);
							}
							break;
					}
				}
				break;
			case osg::PrimitiveSet::DrawElementsUShortPrimitiveType:
//				std::cout << "DrawElementsUShort" << std::endl;
				{
					osg::DrawElementsUShort *de = dynamic_cast<osg::DrawElementsUShort *>(ps);
					CHECK_POINTER(de)
					switch(ps->getMode()){
						case osg::PrimitiveSet::POINTS:
						case osg::PrimitiveSet::LINES:
						case osg::PrimitiveSet::LINE_STRIP:
						case osg::PrimitiveSet::LINE_LOOP:
							std::cerr << "Primitive not supported : " << ps->getMode() << std::endl;
							break;
						case osg::PrimitiveSet::TRIANGLES:
							for( osg::DrawElementsUShort::iterator it=de->begin(); it!=de->end(); ){
								polygon_counts.append(3);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::TRIANGLE_STRIP:
							for( unsigned int j = 0; j < de->getNumIndices() - 2; j++ ){
								polygon_counts.append(3);
								if(ccw){
									polygon_connects.append(de->index(j));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j+2));
									ccw = false;
								} else {
									polygon_connects.append(de->index(j+2));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j));
									ccw = true;
								}
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::TRIANGLE_FAN:
							for( unsigned int j = 0; j < de->getNumIndices() - 2; j++ ){
								polygon_counts.append(3);
								polygon_connects.append(de->index(0));
								polygon_connects.append(de->index(j+1));
								polygon_connects.append(de->index(j+2));
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::QUADS:
							for( osg::DrawElementsUShort::iterator it=de->begin(); it!=de->end(); ){
								polygon_counts.append(4);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::QUAD_STRIP:
							for( unsigned int j = 0; j < (de->getNumIndices()/2)-1; j+=2){
								polygon_counts.append(4);
								if(ccw){
									polygon_connects.append(de->index(j));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j+2));
									polygon_connects.append(de->index(j+3));
									ccw = false;
								} else {
									polygon_connects.append(de->index(j+3));
									polygon_connects.append(de->index(j+2));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j));
									ccw = true;
								}
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::POLYGON:
							polygon_counts.append(de->getNumIndices());
							num_polygons++;
							for( osg::DrawElementsUShort::iterator it=de->begin(); it!=de->end(); ){
								polygon_connects.append(*it++);
							}
							break;
					}
				}
				break;
			case osg::PrimitiveSet::DrawElementsUIntPrimitiveType:
//				std::cout << "DrawElementsUInt" << std::endl;
				{
					osg::DrawElementsUInt *de = dynamic_cast<osg::DrawElementsUInt *>(ps);
					CHECK_POINTER(de)
					switch(ps->getMode()){
						case osg::PrimitiveSet::POINTS:
						case osg::PrimitiveSet::LINES:
						case osg::PrimitiveSet::LINE_STRIP:
						case osg::PrimitiveSet::LINE_LOOP:
							std::cerr << "Primitive not supported : " << ps->getMode() << std::endl;
							break;
						case osg::PrimitiveSet::TRIANGLES:
							for( osg::DrawElementsUInt::iterator it=de->begin(); it!=de->end(); ){
								polygon_counts.append(3);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::TRIANGLE_STRIP:
							for( unsigned int j = 0; j < de->getNumIndices() - 2; j++ ){
								polygon_counts.append(3);
								if(ccw){
									polygon_connects.append(de->index(j));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j+2));
									ccw = false;
								} else {
									polygon_connects.append(de->index(j+2));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j));
									ccw = true;
								}
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::TRIANGLE_FAN:
							for( unsigned int j = 0; j < de->getNumIndices() - 2; j++ ){
								polygon_counts.append(3);
								polygon_connects.append(de->index(0));
								polygon_connects.append(de->index(j+1));
								polygon_connects.append(de->index(j+2));
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::QUADS:
							for( osg::DrawElementsUInt::iterator it=de->begin(); it!=de->end(); ){
								polygon_counts.append(4);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								polygon_connects.append(*it++);
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::QUAD_STRIP:
							for( unsigned int j = 0; j < (de->getNumIndices()/2)-1; j+=2){
								polygon_counts.append(4);
								if(ccw){
									polygon_connects.append(de->index(j));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j+2));
									polygon_connects.append(de->index(j+3));
									ccw = false;
								} else {
									polygon_connects.append(de->index(j+3));
									polygon_connects.append(de->index(j+2));
									polygon_connects.append(de->index(j+1));
									polygon_connects.append(de->index(j));
									ccw = true;
								}
								num_polygons++;
							}
							break;
						case osg::PrimitiveSet::POLYGON:
							polygon_counts.append(de->getNumIndices());
							num_polygons++;
							for( osg::DrawElementsUInt::iterator it=de->begin(); it!=de->end(); ){
								polygon_connects.append(*it++);
							}
							break;
					}
				}
				break;
			default:
				std::cerr << "FATAL ERROR : It is impossible that you reach this point" << std::endl;
				break;
		}
	}


	if(num_polygons > 0) {
		// Create Maya object
		MFnMesh mfn_mesh;
/*		std::cout << "CREATING MESH with " << num_vertices << " vertices, " << num_polygons << " polygons" 
			<< std::endl << " vertex_array (" << vertex_array.length() << ")=" << vertex_array << std::endl
			<< " polygon_counts (" << polygon_counts.length() << ")=" << polygon_counts << std::endl
			<< " polygon_connects (" << polygon_connects.length() << ")=" << polygon_connects << std::endl;*/
		MObject mesh = mfn_mesh.create( num_vertices,
										num_polygons,
										vertex_array,
										polygon_counts,
										polygon_connects,
										_parent);
//		std::cout << "Mesh created!" << std::endl;

		// Texture coordinates
		//std::cout << "Texture coordinates : " << geometry->getNumTexCoordArrays() << " array(s)" << std::endl;
		for(unsigned int i=0; i<geometry->getNumTexCoordArrays(); i++){
			osg::Vec2Array *tcarray = dynamic_cast<osg::Vec2Array *>(geometry->getTexCoordArray(i));
			CHECK_POINTER(tcarray)
			MFloatArray u_array;
			MFloatArray v_array;
			for(unsigned int j=0; j<tcarray->size(); j++){
				u_array.append( (*tcarray)[j].x() );
				v_array.append( (*tcarray)[j].y() );
			}
			MString name_uvset;
			name_uvset.set(i);
			err = mfn_mesh.createUVSet(name_uvset);
			ERROR_CHECK("createUVSet");
			err = mfn_mesh.setUVs(u_array, v_array, &name_uvset);
			ERROR_CHECK("setUVs");
			err = mfn_mesh.assignUVs(polygon_counts, polygon_connects, &name_uvset);
			ERROR_CHECK("assignUVs");
			mfn_mesh.setCurrentUVSetName(name_uvset);
		}

		// Normals
/*		if ( num_normals > 0 ){
			switch ( geometry->getNormalBinding() ){
				case osg::Geometry::BIND_OFF:
					break;
				case osg::Geometry::BIND_OVERALL:
				case osg::Geometry::BIND_PER_VERTEX:
					std::cout << "########" << std::endl;
					std::cout << num_vertices << " vertices " << polygon_counts.length() << " polygons " <<
						polygon_connects.length() << " connections " << normal_array.length() << " normals " <<
						normal_indices.length() << " normal indices" << std::endl;
					std::cout << "########" << std::endl;
//					err = mfn_mesh.setVertexNormals(normal_array, normal_indices);
					MIntArray face_list;
					int ind_face=0;
					for(unsigned int i=0; i<polygon_counts.length(); i++){
						for(int k=0; k<polygon_counts[i]; k++)
                            face_list.append(ind_face);
						ind_face++;
					}
					std::cout << "face_list with " << face_list.length() << " elements" << std::endl;
					std::cout << "vertex_list with " << polygon_connects.length() << " elements" << std::endl;
					err = mfn_mesh.setFaceVertexNormals(normal_array, face_list, polygon_connects);
					ERROR_CHECK("establishing per vertex normals");
					std::cout << "#### " << mfn_mesh.numNormals() << " normals" << std::endl;

//					for(unsigned int i=0; i<normal_array.length(); i++){
//						std::cout << "#" << i << "(" << normal_indices[i] << ") = " << normal_array[i] << std::endl;
//					}

					break;
/*				case osg::Geometry::BIND_PER_PRIMITIVE:
				case osg::Geometry::BIND_PER_PRIMITIVE_SET:
					std::cout << "########" << std::endl;
					std::cout << num_vertices << " vertices " << polygon_counts.length() << " polygons " <<
						polygon_connects.length() << " connections " << perface_normal_indices << " perface_normal_indices " <<
						pervertex_normal_indices << " pervertex_normal_indices" << std::endl;
					std::cout << "########" << std::endl;
					err = mfn_mesh.setFaceVertexNormals(normal_array, perface_normal_indices, pervertex_normal_indices);
					ERROR_CHECK("establishing per vertex and per face normals");
					break;*/
/*			}
		}*/


		// Apply a Shading Engine
		MFnSet shading_group(shading_engine);
		shading_group.addMember(mesh);

	}
	// OSG drawables have no name, so we let Maya to assign a default one
}

void ImporterVisitor::importShapeDrawable(osg::ShapeDrawable *shape_drawable, MObject &shading_engine)
{
	std::cout << "Importing osg::ShapeDrawable [TO BE IMPLEMENTED... SOON]" << std::endl;
}


/**
 *	Create Maya Shading Engine from OSG StateSet
 *
 *	@todo TO-DO... by now it assign default material to be able to see the geometry
 */
void ImporterVisitor::createShadingEngine( osg::StateSet *ss, MObject &shading_engine )
{
//####################################################################
	MSelectionList sel;
	MGlobal::getSelectionListByName("initialShadingGroup", sel);
	if ( sel.length() != 1 )
		std::cout << "WEIRD..." << std::endl;
	else 
		sel.getDependNode(0, shading_engine);
//####################################################################
	return;

	shading_engine = ProxyMaterials::getShader(ss);
}
