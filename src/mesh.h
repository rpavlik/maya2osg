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
#ifndef _MESH_H_
#define _MESH_H_


#include <map>
#include <string>

#include "dagnode.h"

#include <maya/MFnMesh.h>

#include <osg/Geometry>
#include <osg/Texture2d>


// VBO Uniform Location of osg::Geometry first Texture Coordinates  ( if present ) is 8
// adding numUVSets we get the starting Uniform Location for additional Vertex Attributes
#define TEXCOORD_START_LOCATION 8

// This is to create a Set of Vertices with ( partially ) unique Attributes
// Rather than passing a compare class to the set, just overwrite the less then operator of the Vertex Structure
struct VertexAttributes
{
	typedef unsigned int uint ;

	int				mIndex ;
	osg::Vec3		Vertex ;
	osg::Vec3		Normal ;
	osg::Vec3		Tangent ;
	osg::Vec3		Binormal ;
	std::vector< osg::Vec4 > Colors ;
	std::vector< osg::Vec2 > TexCoords ;

	// Colors are not included in operator, as there is only one color per vertex per colorSet
	bool operator < ( const VertexAttributes & other ) const  {
		if			( Vertex < other.Vertex ) return true  ;
		else	if	( other.Vertex < Vertex ) return false ;
		else	if	( Normal < other.Normal ) return true  ;
		else	if	( other.Normal < Normal ) return false ;
		else	return TexCoords < other.TexCoords ;

	}
} ;


// in case of Skinning : 
// data of the Input  Mesh mDagPathOrig is required for the rigGeometry Vertex Positions, Normals, UVs
// BUT will have only the Maya default Shader ( lambert 1 ) attached, while output Mesh could have Multiple Shaders
// -> use Same Structure as output Mesh ( UVs, Normals will most probably be the same, but not Shader Attachment ), with Values of Input Mesh
// if there is no Skinning ( Morphing ) attached, the same MDagPath will be used for both arguments


class Mesh {

public:
	typedef unsigned int uint ;

	static osg::ref_ptr< osg::Node > exporta( MDagPath & mDagPath ) ;

	static std::vector< std::vector< int > > getMVertexToVBOIndices()  {   return mVertexToVBOIndices ;  }


private:
	// NOT REQUIRED DUE VBO WORKFLOW 
	/// Map of OSG Texture Coordinate Sets by Maya UV set name
	//typedef std::map< std::string , osg::ref_ptr<osg::Vec2Array > > TCSetsMap;

	/// Map of OSG Texture Coordinate Sets Indices by Maya UV set name
	//typedef std::map< std::string , osg::ref_ptr< osg::UIntArray > > TCSetsIndicesMap;

	/// Set of Vertices with Unique VertexAttributes
	typedef std::set< VertexAttributes > VertexSet ;

	/// 3 Component Arrays of VertexAttributes ( Triangle )
	typedef std::vector< VertexAttributes > Triangle ;

	/// Array of 3 Component Arrays of VertexAttributes ( Array of Triangles )
	typedef std::vector< std::vector< VertexAttributes > > TriangleArray ;

	/// std::Vector, size of Maya Mesh Idices, with Int vector per Maya Index which holds VBO Indices
	typedef std::vector< std::vector< int > > VertexToVBOIndices ;


	static uint				numVertices ;
	static uint				numUVSets ;
	static uint				numColorSets ;
	static MStringArray	mColorSetNames ;
	static osg::Vec4		osgColor ;
	static bool				smallestDataStorage ;
	
	static VertexToVBOIndices	mVertexToVBOIndices ;

	static void createGeodeStateSettings( MDagPath & mDagPath , osg::Geode * geode ) ;
	static osg::Geometry * createGeometry( VertexSet & vertices , TriangleArray & triangles , bool hasNormalMapping ) ;
/*
	static MObject findShader( const MObject & setNode ) ;
	static osg::ref_ptr< osg::StateSet > getShader( const MObject & shaderNode , osg::ref_ptr< osg::Geometry > & geometry ) ;
	static bool		hasAlphaInputs( MFnDependencyNode & mfnShaderNode ) ;
	static float	getMaterialAttribute( const MPlug & mPlugShader ) ;
	static float	getMaterialAttribute( const MFnDependencyNode & mfnShaderNode , const MString & attr ) ;
	static void		getStateSetTexture( osg::ref_ptr< osg::StateSet > stateSet , const std::string & texName , bool texRepeat ) ;
	static osg::ref_ptr< osg::Texture2D > createTexture( const std::string & texName , bool texRepeat ) ;
*/
} ;


#endif //_MESH_H_


