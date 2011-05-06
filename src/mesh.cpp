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
#include "utility.h"

#include "GLSL/texturingconfig.h"
#include "GLSL/shadingnetwork.h"
#include "config.h"
#include "lights.h"
#include "shadows.h"

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/CullFace>
#include <osg/LightModel>

#include <osgDB/Output>
#include <osgDB/FileNameUtils>

#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MPlug.h>
#include <maya/MFileObject.h>





/*
// ----- Maya -----------------------------------------------------------------------
#include <maya/MPlug.h>
#include <maya/MFnSet.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MIntArray.h>
#include <maya/MPlugArray.h>
#include <maya/MPointArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyGraph.h>


// ----- OSG ------------------------------------------------------------------------
#include <osg/Geode>
#include <osg/TexEnv>
#include <osg/Material>
*/




///////////////////////////////////////////////////////////////////////////////////////////////////
// - TO DO ----------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////

// In the case of one Skinned Maya Shape with Multiple Shaders ( -> multiple osg::Geometries ) :
//  - Pass the Geometries ( Vector ) to skinning module, along with mVertexToVBOIndices 
// Split Shape into Triangles, Quads and Polygons, export multiple DrawArrays, DrawElements per Geometry
// Check for ColorPerVertex Configuration ( RGBA , RGB , A )
// Check Blind Data, export Float, Double, Integer Data as corresponding osg::Array

extern osgDB::Output logFile ;

uint				Mesh::numVertices = 0 ;
uint				Mesh::numUVSets = 0 ;
uint				Mesh::numColorSets = 0 ;
osg::Vec4		Mesh::osgColor( 1.0 , 1.0 , 1.0 , 1.0 ) ;
MStringArray	Mesh::mColorSetNames ;
bool				Mesh::smallestDataStorage = true ;

// No Map, as we access the weights on a per Maya index basis in the Skinning Module, 
// and copy them into the VBO Indicies, hence we store ( multiple ) osg::VBO Indicies per Maya Index
std::vector< std::vector< int > > Mesh::mVertexToVBOIndices ;


/**
 *	Create OSG geometry for a Maya Mesh
 *	Returna pointer to an osg::Geode containing an osg::Drawable
 *	of type osg::Geometry with the information about vertices,
 *	primItives and their attributes.
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


///////////////////////////////////////////////////////////////////////////////////////////////////
// - Create Drawable ------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////

// old Args - osg::ref_ptr< osg::Node > Mesh::exporta( MObject & mObject )
osg::ref_ptr< osg::Node > Mesh::exporta( MDagPath & mDagPath )  {

	// in case of Skinning : 
	// data of the Input  Mesh mDagPathOrig is required for the rigGeometry Vertex Positions, Normals, UVs
	// BUT will have only the Maya default Shader ( lambert 1 ) attached, while output Mesh could have Multiple Shaders
	// -> use Same Structure as output Mesh ( UVs, Normals will most probably be the same, but not Shader Attachment ), with Values of Input Mesh
	// if there is no Skinning ( Morphing ) attached, the same MDagPath will be used for both arguments

	MStatus mStatus ;
	//MDagPath path = mDagPath ;

	uint instanceNumber = 0 ;
	if ( mDagPath.isInstanced() )
		instanceNumber = mDagPath.instanceNumber() ;

	// Here further Code is required : Create a Structure, that holds all required Information per MDagPath
	// If this Mesh is instanced, we need some more Info about configuration of the shader attachment
	// in OSG lowest possible attachmentpoint of a shader is osg::Geometry
	// So, if in MAYA multiple Shaders are attached on a per Polygon Basis, each attachment needs its own osgGeometry and we need to know:
	// 1. Shader Attachment is the same for all Instances, we can instance the resulting osg::Geode as well ( which holds all Geometries )
	// 2. Shader Attachment is different so that Poly Count and Indices per Shader Set don't match, we need to create new Geometries per Shader
	// 3. different Shaders are attached on the same Poly Count and Indices, Attach osg::Shaders on a Geode Basis, and reuse Geometries
	// In case of Multiple UV Sets, check MFnMesh::getUVSetFanilyNames, "A uv set family is a set of per-instance sets ... "
	// for now no additional checking will happen, and each instance will get its own osg::Geode with all associated ( as in 2. ) 

	MFnMesh mfnMesh( mDagPath ) ;
	numVertices = mfnMesh.numVertices() ;
	const std::string shapeName = mfnMesh.name().asChar() ;

	std::map< int , int > mapVtxGlobalLocal ;		// Maps from Master Vertex Index to Face Local Index

	std::string shapeNameMinus = "" ;
	for( uint i = 0 ; i < shapeName.length() ; ++i )  shapeNameMinus += "-" ;

	logFile << std::endl << shapeNameMinus << "--------------------------------------------" << std::endl ;
	logFile << "--- Processing polyShape \"" << shapeName << "\" Instance : " << instanceNumber << " ---" ;
	logFile << std::endl << shapeNameMinus << "--------------------------------------------" << std::endl ;
	cout    << "Geode \"" << shapeName << "\" with Geometries !" << std::endl ;


	// Create the Geode, corresponding to Maya ShapeNode
	osg::ref_ptr< osg::Geode > geode = new osg::Geode() ;
	geode -> setName( shapeName ) ;

	// Attach Geode State Attributes
	createGeodeStateSettings( mDagPath , geode.get() ) ;



	// --- Shaders Attached ---
	logFile << std::endl << "--- Shaders Attached ---" << std::endl ;
	MObjectArray	mShadersAttached ;		// All Shaders attached to this Instance
	MIntArray		mShaderOfPolygon ;		// Length of PolyCount, holds pointer into mShadersAttached

	mStatus = mfnMesh.getConnectedShaders( instanceNumber , mShadersAttached , mShaderOfPolygon ) ;

	if ( mStatus != MS::kSuccess )
		logFile << "ERROR : MFnMesh::getConnectedShaders()" << std::endl ;

	uint				numShaders = mShadersAttached.length() ;

	logFile << numShaders << " Shaders attached !" << std::endl ;

	for( uint i = 0 ; i < numShaders ; ++i )
		logFile << i << ". " << Shader::getSurfaceShaderName( mShadersAttached[ i ] ).asChar() << std::endl ;



	// --- UV Sets ---
	logFile << std::endl << "--- UV Sets ---" << std::endl ;
	bool				ignoreEmptyUVSets = false ;
	bool				logUnmappedPolygons = true ;
	MStringArray	mUVSetNames ;

	numUVSets = mfnMesh.numUVSets() ;
	mStatus = mfnMesh.getUVSetNames( mUVSetNames ) ;

	if ( mStatus != MS::kSuccess )  {
		logFile << "ERROR: MFnMesh::getUVSetNames() !" << std::endl ;
		logFile << "Export Stopped, returning NULL !" ;
		return NULL ;
	}

	logFile << numUVSets << " UVSets found ! " ;
	logFile << ( ignoreEmptyUVSets ? "Ignoring" : "Not ignoring" ) << " empty Sets" << std::endl ;

	for( uint ts = 0 ; ts < numUVSets ; ++ts )  {
		logFile << ts << ". \"" << mUVSetNames[ ts ].asChar() << "\"" ;
		if ( !mfnMesh.numUVs( mUVSetNames[ ts ] , & mStatus ) )  {
			logFile << " - empty" ;
			if ( ignoreEmptyUVSets )  logFile << " - ignoring !" ; 
		}
		logFile << std::endl ;
	}



	// --- UV Set Texture Association ---
	logFile << std::endl << "--- UV Set Texture Association ---" << std::endl ;

	TexturingConfig	texturingConfig ;		// Store Maya UVSet( = osg Texture Unit ) Texture Association 
	MObjectArray		mTextureArray ;		// Stores all Textures per UV Set per Poly Shape 
	MFileObject			mFileObject ;			// Manipulate File Path String, to get File Name without Path
	std::string			texName ;				// Feed into TexturingConfig with a Texture Unit

	for( uint ts = 0 ; ts < numUVSets ; ++ts )  {	// ts = TextureSet
		logFile << ts << ". \"" << mUVSetNames[ ts ].asChar() << "\" : " ;

		mfnMesh.getAssociatedUVSetTextures( mUVSetNames[ ts ] , mTextureArray ) ;

		for( uint i = 0 ; i < mTextureArray.length() ; ++i )  {
			// Only interested in file textures
			if ( mTextureArray[ i ].hasFn( MFn::kFileTexture ) )  {
				MFnDependencyNode mfnDependencyTexture( mTextureArray[ i ] ) ;

				// Get the Texture Node Name
				texName = mfnDependencyTexture.name( & mStatus ).asChar() ;
				texturingConfig.setTCSet( texName , ts ) ;

				// Get the Image File Name
				mStatus = mFileObject.setRawFullName( mfnDependencyTexture.findPlug( "fileTextureName" , & mStatus ).asString() ) ;

				logFile << "\"" << texName << "( " << mFileObject.rawName().asChar() << " )\" , " ;

			}
		}
		logFile << std::endl ;
	}



	// --- Color Per Vertex ---
	logFile << std::endl << "--- Color Per Vertex ---" << std::endl ;
	bool				ignoreEmptyColorSets = false ;
	numColorSets = mfnMesh.numColorSets() ;
	mStatus = mColorSetNames.clear() ;
	mStatus = mfnMesh.getColorSetNames( mColorSetNames ) ;

	if ( mStatus != MS::kSuccess )  {
		logFile << "ERROR: MFnMesh::getColorSetNames() !" << std::endl ;
		logFile << "Export Stopped, returning NULL !" ;
		return NULL ;
	}

	logFile << numColorSets << " Color Sets found ! " ;
	logFile << ( ignoreEmptyColorSets ? "Ignoring" : "Not ignoring" ) << " empty Sets" << std::endl ;

	for( uint i = 0 ; i < numColorSets ; ++i )   {
		logFile << i << ". \"" << mColorSetNames[ i ].asChar() << "\"" ;
		if ( !mfnMesh.numColors(  mColorSetNames[ i ] , & mStatus ) )  {
			logFile << " - empty" ;
			if ( ignoreEmptyColorSets )  logFile << " - ignoring !" ; 
		}
		logFile << std::endl ;
	}

	// Get Reference to all Color Sets Color Values ( Arrays size Maya Vertex Index )
	std::vector< MColorArray > mColorSets( numColorSets ) ;
	MColor mDefaultColor( osgColor.r() , osgColor.g() , osgColor.b() , osgColor.a() ) ;
	for( uint cs = 0 ; cs < numColorSets ; ++cs )  {
		MString colorSetName = mColorSetNames[ cs ] ;
		mStatus = mfnMesh.getColors( mColorSets[ cs ] , & mColorSetNames[ cs ] , & mDefaultColor ) ;
	}

	uint numColorSetNames = mColorSetNames.length() ;

	if ( mStatus != MS::kSuccess )  {
		logFile << "ERROR: MFnMesh::getColors() !" << std::endl ;
		logFile << "Export Stopped, returning NULL !" ;
		return NULL ;
	}



	// --- Flip Normals If Opposite ---
	logFile << std::endl << "--- Flip Normals Surface Mode is Opposite ---" << std::endl ;
	bool					opposite = false ;
	MFnDependencyNode mfnDependencyNode( mDagPath.node( & mStatus ) ) ;
	MPlug					mPlug = mfnDependencyNode.findPlug( "opposite" , & mStatus ) ;

	if ( mStatus == MS::kSuccess )  mPlug.getValue( opposite ) ;
	else logFile << "ERROR: MPlug::getValue( \"opposite\" ) !" << std::endl ;

	opposite = opposite && Config::instance() -> getFlipNormalsIfOpposite() ;

	logFile << "Normals will " << ( opposite ? "" : "NOT" ) << " be flipped !" << std::endl ;
	float normalDirection( opposite ? -1.0f : 1.0f ) ;



	// --- Export Geometry per Shader ---
	logFile << std::endl << std::endl << "--- Export Geometry per Shader ---" << std::endl ;
	logFile << "----------------------------------" << std::endl << std::endl ;

	//	create data arrays for current drawable
	VertexSet		vertices ;
	Triangle			triangle( 3 ) ;
	TriangleArray	triangles ;

	// Set the count of Colors / Vertex Attributes per Vertex and UVs per Vertex
	// This is the same for all Geometries which will be generated
	for( uint tv = 0 ; tv < 3 ; ++tv )  {
		triangle[ tv ].Colors.resize( numColorSets ) ;
		triangle[ tv ].TexCoords.resize( numUVSets ) ;
	}

	//	create temp data objects for Vertex Attribute Data
	osg::Vec2	osgVec2 ;
	osg::Vec3	osgVec3 ;
	osg::Vec4	osgVec4 ;

	// create data arrays for eventual Tangents and Binormals
	MFloatVectorArray mTangentArray , mBinormalArray ;

	// For each attached Shader
	for( unsigned sa = 0 ; sa < mShadersAttached.length() ; ++sa )  {		// sa = Shaders Attached

		// Clear Data Structures
		vertices.clear() ;
		triangles.clear() ;

		const std::string shaderName = Shader::getSurfaceShaderName( mShadersAttached[ sa ] ).asChar() ;
		logFile << std::endl << "--- Processing Polygons attached to : \"" << shaderName << "\" ---" << std::endl ;
		for( uint i = 0 ; i < shapeName.length() ; ++i )  logFile << "-" ;
		logFile << "---------------------------------------" << std::endl ;



		// --- Normal Mapping ---
		logFile << std::endl << "--- Normal Mapping ---" << std::endl ;

		// First get query Config Settings and log result
		bool hasNormalMapping =	Config::instance() -> getUseGLSL() && Config::instance() -> getEnableBumpMapping() ;

		if ( !hasNormalMapping )
			logFile << "Ignoring due to Config Settings ( Use GLSL or Enable BumpMapping ) !" << std::endl ;

		else	// Query whether a BumpMap is attached to the current Shader
			hasNormalMapping = hasNormalMapping && Shader::connectedChannel( Shader::getSurfaceShader( mShadersAttached[ sa ] ) , "normalCamera" ) ;

		if ( !hasNormalMapping )
			logFile << "No Bump/Normal Map Attached !" << std::endl ;

		else  {

			// Find the Texture connected to surfaceShader.normalCamera
			MObject mBumpNode , mBumpTexture ;
			Shader::getNodeConnectedToChannel( Shader::getSurfaceShader( mShadersAttached[ sa ] ) , "normalCamera" , mBumpNode ) ;
			Shader::getNodeConnectedToChannel( mBumpNode , "bumpValue" , mBumpTexture ) ;
			MFnDependencyNode mfnDependencyTexture( mBumpTexture ) ;

			hasNormalMapping = mBumpTexture.hasFn( MFn::kFileTexture )  ;

			// Check if a File Texture is attached to surfaceSahder.normalCamera ( via bump2d Node )
			if ( !hasNormalMapping )
				logFile << "No File Texture attached to Surface Shader Bump Map Attribute !" << std::endl ;
			
			else  {
				// with the Name access the Texture Unit from texturingConfiguration
				int idxBumpTexture = texturingConfig.getTCSet( mfnDependencyTexture.name().asChar() ) ;

				// the Texture Unit corresponds to the uvSetNames Index, required for the Tangent Space access
				mStatus = mfnMesh.getTangents(  mTangentArray  , MSpace::kObject , & mUVSetNames[ idxBumpTexture ] ) ;
				mStatus = mfnMesh.getBinormals( mBinormalArray , MSpace::kObject , & mUVSetNames[ idxBumpTexture ] ) ;

				logFile << "Bump Map Attached, generating Tangents and Binormals for UVSet \"" << mUVSetNames[ idxBumpTexture ].asChar() << "\"" << std::endl ;
			}		
		}



		MItMeshPolygon mItMeshPolygon( mDagPath ) ;

		// We iterate through each Polygon, which can be 3 to n Sided
		// With the method mItMeshPolygon.getTriangles( mTrisPointArray , mTrisIndexArray )
		// we get the internal Maya Triangle representation of the Polygon 
		for( ; !mItMeshPolygon.isDone() ; mItMeshPolygon.next() )  {	// Loop through each Polygon of the Mesh

			// Just continue, if current polygon is not attached to the Shader we are processing
			if ( sa != mShaderOfPolygon[ mItMeshPolygon.index() ] )  continue ;		// sa = Shaders Attached

			// Create Storage for :
			// Global Vertex Indices of Current Polygon
			// U V Values of Current Polygons Vertices, store in two float Arrays
			// Normals of Current Polygons Vertices
			MIntArray		mFaceIndexArray ;					// Global Vertex Indices of the Current Face
			MFloatArray		mFaceUArray , mFaceVArray ;	// U V Values per Vertex of the Current Face
			MVectorArray	mFaceNormalArray ;				// Normals    per Vertex of the Current Face

			// If Faces consist of more than 3 Points, get Indices and Points of Triangles that build the current Face
			MIntArray		mTrisIndexArray ;					// Global Vertex Indices of Triangles building the Current Face
			MPointArray		mTrisPointArray ;					// Points per Vertex per Triangle

			// Grab the data
			mStatus = mItMeshPolygon.getVertices( mFaceIndexArray ) ;
			mStatus = mItMeshPolygon.getNormals( mFaceNormalArray , MSpace::kObject ) ;

			// Get internal Maya Triangle Representation of current Polygon )
			// Store Positions in mTrisPointArray, size n ( count of polygon Vertices )
			// Store Global Indices in mTrisIndexArray, size n ( count of polygon Vertices )
			mStatus = mItMeshPolygon.getTriangles( mTrisPointArray , mTrisIndexArray , MSpace::kObject ) ;

			// Map from Vertex Global Indices to Vertex Polygon Indices, to access Array Data
			mapVtxGlobalLocal.clear() ;
			for( unsigned int vi = 0 ; vi < mFaceIndexArray.length() ; ++vi )  {		// vi = VertexIndex
				mapVtxGlobalLocal[ mFaceIndexArray[ vi ] ] = vi ;
			}

			// bool array if the current polygon is mapped, index correspond to mUVSetsNames Indices
			std::vector< bool > isMappedInSet ;
			for( uint ms = 0 ; ms < numUVSets ; ++ms )  {	// ms = MappedInSet
				isMappedInSet.push_back( mItMeshPolygon.hasUVs( mUVSetNames[ ms ] , & mStatus ) ) ;
				if ( logUnmappedPolygons && !isMappedInSet[ ms ] )  {
					logFile << "WARNING : found unmapped polygon in Set \"" << mUVSetNames[ ms ].asChar() << "\" ! " ;
					logFile << "FaceIndex : " << mItMeshPolygon.index() << std::endl ;
				}
			}


			// Grab Data out of captured Storage
			// Populate VertexAttribute Data into Triangle Structure
			// Insert VertexAttribute Structure into Set
			// Append Triangle Structure to Triangles Vector

			int	colorIndex ;
			uint	globalIndex , localIndex , tangentIndex ;
			uint	tc , tv , vc ;	// TriangleCounter ( per Polygon ) , TriangleVertex , VectorComponent
			uint	numTrisIndices = mTrisIndexArray.length() ;

			// Every 3 Points in Index/PointArray build a Triangle, so for each three steps create custom triangle
			// Per Vertex, grab all Maya Data and push into Vertex
			for( tc = 0 ; tc < numTrisIndices ; tc += 3 )  {	// tc = triangleCounter
				for( tv = 0 ; tv < 3 ; ++tv )  {						// tv = triangleVertex
					globalIndex = mTrisIndexArray[ tc + tv ] ;
					triangle[ tv ].mIndex = globalIndex ;

					// Position
					assign( osgVec3 , mTrisPointArray[ tc + tv ] ) ;
					for( vc = 0 ; vc < 3 ; ++vc )										// vc = VectorComponent
						if ( inTolerance( 0.0f , osgVec3[ vc ] , 1.0e-15f ) )	// if Value small
							osgVec3[ vc ] = 0.0f ;										// round to 0.0
					triangle[ tv ].Vertex = osgVec3 ;

					// Normal
					localIndex  = mapVtxGlobalLocal[ globalIndex ] ;			// Access the Local Index through the map
					assign( osgVec3 , mFaceNormalArray[ localIndex ] ) ;
					for( vc = 0 ; vc < 3 ; ++vc )										// vc = VectorComponent
						if ( inTolerance( 0.0f , osgVec3[ vc ] , 1.0e-15f ) )	// if Value small
							osgVec3[ vc ] = 0.0f ;										// round to 0.0
					osgVec3.normalize() ;
					triangle[ tv ].Normal = osgVec3 * normalDirection ;

					// Tangent and Binormal
					if ( hasNormalMapping )  {
						tangentIndex = mItMeshPolygon.tangentIndex( localIndex , & mStatus ) ;
						assign( osgVec3 ,  mTangentArray[ tangentIndex ] ) ;
						triangle[ tv ].Tangent = osgVec3 ;
						assign( osgVec3 , mBinormalArray[ tangentIndex ] ) ;
						triangle[ tv ].Binormal = osgVec3 ;
					}

					// Loop Through Color Sets
					for( uint cs = 0 ; cs < numColorSets ; ++cs )  {			// cs = Color Set
						// Vertices don't store colors, but rather indicies into separate color arrays ( so length does not need to be vertex count )
						// So first get this Index into colorIndex, and access the Color array with it
						// colorIndex is -1 if the Vertex has no Color assigned, so we assign the default osgColor
						mItMeshPolygon.getColorIndex( localIndex , colorIndex , & mColorSetNames[ cs ] ) ;
						if ( colorIndex < 0 )  triangle[ tv ].Colors[ cs ] = osg::Vec4( osgColor ) ;
						else  {
							assign( osgVec4 , mColorSets[ cs ][ colorIndex ] ) ;
							for( vc = 0 ; vc < 4 ; ++vc )										// vc = VectorComponent
								if ( inTolerance( 0.0f , osgVec4[ vc ] , 1.0e-15f ) )	// if Value small
									osgVec4[ vc ] = 0.0 ;										// round to 0.0
						}
						triangle[ tv ].Colors[ cs ] = osgVec4 ;
					}

					// Loop Through UVSets
					for( uint ts = 0 ; ts < numUVSets ; ++ts )  {				// ts = TextureCoordinate Set
						if ( isMappedInSet[ ts ] )  {
							// Get UVs for the whole Polygon -> optimize, as this gets called per Triangle of a Polygon
							mStatus = mItMeshPolygon.getUVs( mFaceUArray , mFaceVArray , & mUVSetNames[ ts ] ) ;
							osgVec2[ 0 ] = mFaceUArray[ localIndex ] ;
							osgVec2[ 1 ] = mFaceVArray[ localIndex ] ;
							for( vc = 0 ; vc < 2 ; ++vc )										// vc = VectorComponent
								if ( inTolerance( 0.0f , osgVec2[ vc ] , 1.0e-15f ) )	// if Value small
									osgVec2[ vc ] = 0.0f ;										// round to 0.0
							triangle[ tv ].TexCoords[ ts ] = osgVec2 ;
						}
						else  triangle[ tv ].TexCoords[ ts ] = osg::Vec2() ;
					}
					vertices.insert( triangle[ tv ] ) ;
				}
				triangles.push_back(  triangle ) ;
			}
		}

		osg::Geometry * geometry = createGeometry( vertices , triangles , hasNormalMapping ) ;
		geometry -> setName( shapeName + "_" + shaderName ) ;
		osg::StateSet * stateSet = geometry -> getOrCreateStateSet() ;



		// --- Shader ---
		logFile << std::endl << "--- Shader ---" << std::endl ;
		if ( Config::instance() -> getUseGLSL() )	 {  // Create the shading network and configure the StateSet
			ShadingNetwork sn( Shader::getSurfaceShader( mShadersAttached[ sa ] ) , texturingConfig , * stateSet ) ;
			logFile << "Using GLSL" << std::endl ;
		}

		else	{  // Configure the StateSet for fixed function OpenGL pipeline
			Shader::exporta( mShadersAttached[ sa ] , * stateSet ) ;
			logFile << "Using Fixed Function Shader" << std::endl ;
		}

		geode -> addDrawable( geometry ) ;

		// --- Result Geometry ---
		logFile << std::endl << "--- Result Geometry ---" << std::endl ;
		logFile << "osg::Geometry \"" << shapeName << "_" << shaderName << "\" created and attached !" << std::endl << std::endl ;

	}

	logFile << shapeNameMinus << "------------------------------" << std::endl ;
	logFile << "--- Mesh node \"" << shapeName << "\" processed ---" << std::endl ;
	logFile << shapeNameMinus << "------------------------------" << std::endl ;

	return geode.release() ;

}



///////////////////////////////////////////////////////////////////////////////////////////////////
// - Add Geode Settings ---------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
void Mesh::createGeodeStateSettings( MDagPath & mDagPath , osg::Geode * geode )  {

	// --- Face Culling ---
	logFile << std::endl << "--- Face Culling ---" << std::endl ;

	// Check whether mesh is single or double sided
	// attributes doubleSided and opposite in surfaceShape node
	// Attach corresponding LightModel ( double Sided or opposite ) and CullFace ( ? opposite )

	MStatus	mStatus ;
	MFnDependencyNode mfnDependencyNode( mDagPath.node( & mStatus ) ) ;
	bool		doubleSided	= mfnDependencyNode.findPlug(  "doubleSided"	, & mStatus ).asBool() ;
	bool		opposite		= mfnDependencyNode.findPlug(  "opposite"		, & mStatus ).asBool() ;

	if ( mStatus != MS::kSuccess )
		logFile << "ERROR: MFnDependencyNode::findPlug( \"doubleSided\" / \"opposite\" )" << std::endl ;

	osg::StateSet * stateSet = geode -> getOrCreateStateSet() ;

	// Order of this operation is important, as we use the original doubleSided first, and tweak it later on
	Config * config = Config::instance() ;		// If true we need Culling and two sided Lighting, if false then only Culling
	opposite =	( config -> getSurfaceMode() == Config::KEEP )		&&		// Only Reachable through Maya Setting
					! config -> getFlipNormalsIfOpposite()					&&		// Not if the opposite State is baked into Normals
					opposite ;																// Queried Plug

	doubleSided = Config::DOUBLE || Config::KEEP &&  doubleSided ;			// If true we need two sided Lighting, but no culling

	if ( doubleSided )	// No Culling Required
		logFile << "Surface Mode Double Sided, no osg::CullFace State Attribute attached !" << std::endl ;

	else  {		// We cull one Face Side
		if ( !opposite )  {
			stateSet -> setAttributeAndModes( new osg::CullFace( osg::CullFace::BACK  ) , osg::StateAttribute::ON ) ;
			logFile << "Surface Mode Single Sided, osg::CullFace::Back State Attribute attached !" << std::endl ;
		}

		else  {
			stateSet -> setAttributeAndModes( new osg::CullFace( osg::CullFace::FRONT ) , osg::StateAttribute::ON ) ;
			logFile << "Surface Mode Single Sided Opposite, osg::CullFace::FRONT State Attribute attached !" << std::endl ;

			if ( mfnDependencyNode.findPlug( "opposite" , & mStatus ).asBool() && config -> getFlipNormalsIfOpposite() )  {
				logFile << "Normals flipped !" << std::endl ;
			}
		}
	}


	if ( opposite || doubleSided )  {	// Lightning needs a fix as otherwise half bright
		osg::ref_ptr< osg::LightModel > lightModel = Lights::getDefaultLightModel() ;
		lightModel -> setTwoSided( true ) ;
		stateSet -> setAttribute( lightModel.get() ) ;
		logFile << "State Attribute osg::LightModel with Two Sided Mode attached !" << std::endl ;
	}



	// --- Shadows ---
	if ( Config::instance() -> getComputeShadows() )  {
		logFile << std::endl << "--- Shadows ---" << std::endl ;

		if ( ! mfnDependencyNode.findPlug( "castsShadows" ).asBool() )  {
			geode -> setNodeMask( geode -> getNodeMask() & ~Shadows::CastsShadowTraversalMask ) ;
			logFile << "Casting Shadows !" << std::endl ;
		}

		if ( ! mfnDependencyNode.findPlug( "receiveShadows" ).asBool() )  {
			geode -> setNodeMask( geode -> getNodeMask() & ~Shadows::ReceivesShadowTraversalMask ) ;
			logFile << "Receiving Shadows !" << std::endl ;
		}
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// - Create OSG Geometry - Unique Vertex Attributes -----------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
osg::Geometry * Mesh::createGeometry( VertexSet & vertices , TriangleArray & triangles , bool hasNormalMapping )  {

	// vertices is a Set of Unique Vertex Attributes
	// triangles is an array of non Unique Vertex Attributes, just brute force 3 Vertices per Triangle
	// Both Structures will used to create an Array with unique VertexAttributes, 
	// and ( Triangle ) Indices pointing into this Array
	logFile << "creating osg::Geometry ( Unique Vertex Attributes )" << std::endl ;

	uint numUniqueVerts = vertices.size() ;

	if ( !numUniqueVerts )  {
		logFile << " WARNING : No Vertices in VertexSet ! No Geometry is constructed !" << std::endl ;
		return NULL ;
	}

	osg::ref_ptr< osg::Geometry > geometry = new osg::Geometry() ;

	geometry -> setUseDisplayList( false ) ;
	geometry -> setUseVertexBufferObjects( true ) ;
	geometry -> setDataVariance( osg::Object::STATIC ) ;

	VertexSet::iterator	itUniqueVerts ;		// Iterates Through Set of Unique Vertices

	std::map< VertexAttributes , int >		mapVertexAttributeIndex ;	// Maps VertexAttribte to Indices, to be accessed in Draw Elementsts

	mVertexToVBOIndices.clear() ;
	mVertexToVBOIndices.resize( numVertices ) ;
	osg::Vec3Array * vrts = new osg::Vec3Array() ;	// Vertices
	osg::Vec3Array * nrms = new osg::Vec3Array() ;	// Normals
	osg::Vec3Array * tgnt = new osg::Vec3Array() ;	// Tangents  ( eventually )
	osg::Vec3Array * bnms = new osg::Vec3Array() ;	// Binormals ( eventually )
	std::vector< osg::Vec4Array * > rgbaVector ;		// Colors and Custom Vertex Attributes
	std::vector< osg::Vec2Array * > texcVector ;		// Texture Coordinates ( eventually multiple )

	uint i , j , cs , ts ;	// Color Set , TextureCoordinate Set

	for( cs = 0 ; cs < numColorSets	; ++cs )  rgbaVector.push_back( new osg::Vec4Array() ) ;	// cs = Color Set
	for( ts = 0 ; ts < numUVSets		; ++ts )  texcVector.push_back( new osg::Vec2Array() ) ;	// ts = TextureCoordinate Set

	for( i = 0 , itUniqueVerts = vertices.begin() ; itUniqueVerts != vertices.end() ; ++itUniqueVerts , ++i )  {
		mapVertexAttributeIndex[ * itUniqueVerts ] = i ;

		mVertexToVBOIndices[ itUniqueVerts -> mIndex ].push_back( i ) ;
		vrts -> push_back( itUniqueVerts -> Vertex ) ;
		nrms -> push_back( itUniqueVerts -> Normal ) ;

		if ( hasNormalMapping )  {
			tgnt -> push_back( itUniqueVerts -> Tangent ) ;
			bnms -> push_back( itUniqueVerts -> Binormal ) ;
		}

		for( cs = 0 ; cs < numColorSets ; ++cs )  rgbaVector[ cs ] -> push_back( itUniqueVerts -> Colors[ cs ] ) ;	// cs = Color Set
		for( ts = 0 ; ts < numUVSets ; ++ts )  texcVector[ ts ] -> push_back( itUniqueVerts -> TexCoords[ ts ] ) ;	// ts = TextureCoordinate Set

	}



	uint numIndices = triangles.size() * 3 ;
	enum DrawElementsSize  {  UByte , UShort , UInt  } ;

	DrawElementsSize drawElementsSize = UInt ;

	if			( smallestDataStorage && numIndices <= 256 )		drawElementsSize = UByte  ;
	else	if	( smallestDataStorage && numIndices <= 65536 )	drawElementsSize = UShort ;

	osg::DrawElements * drawElements ;

	logFile << "Required index count " << numIndices ;

	switch( drawElementsSize )  {
	case UByte :
		drawElements = new osg::DrawElementsUByte( osg::PrimitiveSet::TRIANGLES ) ;
		static_cast< osg::DrawElementsUByte * >( drawElements ) -> reserve( numIndices ) ;
		logFile << " is less then equal 256 : using DrawElementsUByte" << std::endl ;
	break ;

	case UShort :
		drawElements = new osg::DrawElementsUShort( osg::PrimitiveSet::TRIANGLES ) ;
		static_cast< osg::DrawElementsUShort * >( drawElements ) -> reserve( numIndices ) ;
		logFile << " is less then equal 65536 : using DrawElementsUShort" << std::endl ;
	break ;

	case UInt :
		drawElements = new osg::DrawElementsUByte( osg::PrimitiveSet::TRIANGLES ) ;
		static_cast< osg::DrawElementsUInt * >( drawElements ) -> reserve( numIndices ) ;
		logFile << "more then 65536 : using DrawElementsUInt" << std::endl ;
	break ;
	}


	int index ;
	for( i = 0 ; i < triangles.size() ; ++i )  {
		for( j = 0 ; j < 3 ; ++j )  {
			index = mapVertexAttributeIndex[ triangles[ i ][ j ] ] ;

			switch( drawElementsSize )  {
			case UByte	: static_cast< osg::DrawElementsUByte	* >( drawElements ) -> push_back( index ) ; break ;
			case UShort	: static_cast< osg::DrawElementsUShort	* >( drawElements ) -> push_back( index ) ; break ;
			case UInt	: static_cast< osg::DrawElementsUInt	* >( drawElements ) -> push_back( index ) ; break ;
			default		: ;
			}
		}
	}

	geometry -> setVertexArray( vrts ) ;
	geometry -> setNormalArray( nrms ) ;
	geometry -> setNormalBinding( osg::Geometry::BIND_PER_VERTEX ) ;

	// Tangent and Binormal Locations as per Definition in shadingnetwork.h
	if ( hasNormalMapping )  {
		geometry -> setVertexAttribArray( TANGENT_ATTRIB_LOCATION , tgnt ) ;
		geometry -> setVertexAttribBinding( TANGENT_ATTRIB_LOCATION , osg::Geometry::BIND_PER_VERTEX ) ;

		geometry -> setVertexAttribArray( BINORMAL_ATTRIB_LOCATION , bnms ) ;
		geometry -> setVertexAttribBinding( BINORMAL_ATTRIB_LOCATION , osg::Geometry::BIND_PER_VERTEX ) ;
	}

	// --- Color Binding ---
	logFile << std::endl << "--- Color Binding ---" << std::endl ;

	// Track how many Maya Colors should result in osg::Goemetry Color Arrays / custom Vertex Attributes,
	// and find out which Uniform Index should be used for the VertexAttributes
	// Use Following Layout : http://forum.openscenegraph.org/viewtopic.php?t=4653
	//	0 vertex position glVertex
	//	1 vertex weights 0-3 glWeightARB, VertexWeightEXT
	//	2 normal glNormal
	//	3 primary color glColor
	//	4 secondary color glSecondaryColorEXT
	//	5 fog coordinate glFogCoordEXT
	//	6 Tangent
	//	7 Binormal
	//	8 texture coordinate set 0 MultiTexCoord(TEXTURE0, ...)
	//	8+n texture coordinate set n MultiTexCoord(TEXTURE0+n, ...)
	// 8+n+m Additional ( m ) Colors ( as Vertex Attributes )
	// 8+n+m+l Additional ( l ) Blind Data

	// Texture coordinates Start at Uniform Index of 8, plus additional Tex Coords
	// If Primary and Secondary Colors are used, we have numColorSets - 2 Vertex Attributes ...
	// ... starting at 8 and going to 7 + numUVSets
	// If either Primary or Secondary Color is used, have numColorSets - 1 Vertex Attributes
	uint numColorArrays = 0 ;	// count of Maya Vertex Colors Sets to osg::Geometry._colorData / secondaryColorData

	bool addColorAsPrimary		= true ;
	bool addColorAsSecondary	= true ;

		// Put the First Maya Color into osg::Geometry::setColorArray
	if ( addColorAsPrimary && numColorSets > 0 )  {
		logFile << "Color Set : \"" << mColorSetNames[ 0 ].asChar() << "\" bound to osg::Geometry._colorData" << std::endl ;
		geometry -> setColorArray( rgbaVector[ 0 ] ) ;
		geometry -> setColorBinding( osg::Geometry::BIND_PER_VERTEX ) ;
		++numColorArrays ;			// Reduce the count of Vertex Attributes by one
	}

	// Put the First OR Second Maya Color into osg::Geometry::setSecondaryColorArray
	// addColorAsPrimary evaluates to 0 or 1
	uint colorArrayOffset = ( uint )addColorAsPrimary ;
	if ( addColorAsSecondary && numColorSets > ( colorArrayOffset ) )  {
		logFile << "Color Set : \"" << mColorSetNames[ colorArrayOffset ].asChar() << "\" bound to osg::Geometry._secondaryColorData" << std::endl ;
		geometry -> setSecondaryColorArray( rgbaVector[ colorArrayOffset ] ) ;
		geometry -> setSecondaryColorBinding( osg::Geometry::BIND_PER_VERTEX ) ;
		++numColorArrays ;			// Reduce the count of Vertex Attributes by one
	}

	// Set all Texture Coordinates which :
	// start at 8 and go to 7 + numUVSets
	for( ts = 0 ; ts < numUVSets ; ++ts )	// ts = TextureCoordinates Sets
		geometry -> setTexCoordArray( ts , texcVector[ ts ] ) ;

	// Set all additional Vertex Attributes which : 
	// start at 8 + numUVSets and go to 7 + numUVSets + numColorSets - numColorArrays
	// Currently, the setNames cannot be associated with units, but this is an osg::Program method ...
	// ... and we don't know if such a program is in use or not, so for now just print the Name and Location
	for( cs = numColorArrays ; cs < numColorSets ; ++cs )	 {	// cs = Color Sets
		uint location = TEXCOORD_START_LOCATION + numUVSets + cs - numColorArrays ;
		logFile << "Color Set : \"" << mColorSetNames[ cs ].asChar() << "\" bound to osg::Geometry._vertexAttribList [ " ;
		logFile << cs - numColorArrays << " ] at Uniform Location : " << location << std::endl ;
		geometry -> setVertexAttribArray( location , rgbaVector[ cs ] ) ;
		geometry -> setVertexAttribBinding( location , osg::Geometry::BIND_PER_VERTEX ) ;
	}

	geometry -> addPrimitiveSet( drawElements ) ;

	return geometry.release() ;
}




/////////////////////
//	- S H A D E R - //
/////////////////////


#if 0
///////////////////////////////////////////////////////////////////////////////////////////////////
//	- Find the shading node for the given shading group mSetAttached node --------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
MObject Mesh::findShader( const MObject & setNode )
{
	MFnDependencyNode  mfnDependencyNode( setNode ) ;
	MPlug shaderPlug = mfnDependencyNode.findPlug( "surfaceShader" , true , & mStatus ) ;

	if ( !shaderPlug.isNull() )  {			
		MPlugArray connectedPlugs ;
		bool asSrc = false ;
		bool asDst = true  ;
		shaderPlug.connectedTo( connectedPlugs , asDst , asSrc ) ;

		if ( connectedPlugs.length() != 1 )
			logFile << "Error getting shader" << std::endl ;
		else 
			return connectedPlugs[ 0 ].node() ;
	}

	return MObject::kNullObj ;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// - Get Maya Shader as osg::StateSet Attribute ---------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr< osg::StateSet > Mesh::getShader( const MObject & shaderNode , osg::ref_ptr< osg::Geometry > & geometry )
{
	// --- To Do ---
	// Improve Lambert Shading Code. If Ambient, Incandescence is set, use an osg::Material
	MFnDependencyNode mfnShaderNode( shaderNode ) ;

	std::string shaderName = mfnShaderNode.name().asChar() ;

	logFile << "ShaderNode Name : " << shaderName << std::endl ;
	logFile << "ShaderNode Type : " << mfnShaderNode.typeName().asChar() << std::endl ;
	//logFile << "ShaderNode kMFn : " << shaderNode.apiTypeStr() << std::endl ;

	// Create only a Ref_Pointer to a stateSaet, create a StateSet later if required 
	osg::ref_ptr< osg::StateSet > stateSet ;

	// search mapShader for the current Maya Shader ( Name )
	std::map< std::string , osg::ref_ptr< osg::StateSet > >::iterator itMapShader = mapShader.find( shaderName ) ;

	// if Success, return the StateSet in Map
	if ( itMapShader != mapShader.end() )  {
		logFile << "Found " << shaderName << " in std::map mapShader, reusing StateSet" << std::endl ;
		return itMapShader -> second ;
	}

	// get Maya Transparency rgb, average and set the a value ( [ 3 ] ) of osgColor
	getMaterialAttribute( mfnShaderNode , "transparency" ) ;
	if ( mStatus != MS::kSuccess )  {
		getMaterialAttribute( mfnShaderNode , "outTransparency" ) ;
		if ( mStatus != MS::kSuccess )  {
			osgColor = osg::Vec4( 0.f , 0.f , 0.f , 0.f ) ;
		}
	}

	osgColor.a() = 1.f - ( osgColor.r() + osgColor.g() + osgColor.b() ) / 3.f ;

	// Create a State Set if Transparency Values are set, and enable Blending
	if ( osgColor.a() < 0.999999f )  {
		if ( !stateSet.get() )  stateSet = new osg::StateSet() ;
		stateSet -> setMode( GL_BLEND , true ) ;
		stateSet -> setRenderingHint( osg::StateSet::TRANSPARENT_BIN ) ;
	}

	else  osgColor.a() = 1.f ;


	MPlug mPlugColor ;	// require the Plug for finding textures
	mPlugColor = mfnShaderNode.findPlug( "color" , true , & mStatus ) ;
	getMaterialAttribute( mPlugColor ) ;

	if ( mStatus != MS::kSuccess )  {
		mPlugColor = mfnShaderNode.findPlug( "outColor" , true , & mStatus ) ;
		getMaterialAttribute( mPlugColor ) ;

		if ( mStatus != MS::kSuccess )  {
			logFile << "WARNING : Shader node of this mSetAttached has neither \"color\" nor \"outColor\" plug! / mPlugColor: mStatus != MS::kSuccess" << std::endl;
			return stateSet ;
		}
	}

	// Get the Diffuse Parameter from lambert and phong, and multiply each of the color channels
	float diffuse = getMaterialAttribute( mfnShaderNode , "diffuse" ) ;

	if ( mStatus == MS::kSuccess )  {
		osgColor.r() *= diffuse ;
		osgColor.g() *= diffuse ;
		osgColor.b() *= diffuse ;
	}

	logFile << "ShaderNode Color and Alpha : " << osgColor[0] << " , " << osgColor[1] << " , " << osgColor[2] << " , " << osgColor[3]<< std::endl ;


	// find out if this Shader is Animated
	bool isAnimated = false ;
	MPlug mPlugAnimated ;
	mPlugAnimated = mfnShaderNode.findPlug( "animated" , true , & mStatus ) ;
	if ( mStatus == MS::kSuccess )  {
		isAnimated = mPlugAnimated.asBool() ;
		geometry -> setUseDisplayList( false ) ;
		geometry -> setUseVertexBufferObjects( true ) ;
		logFile << "is Animated" << std::endl ;

		// If the transparency or outTransparency Channels do have inputs, enable Blending
		// Assuming that the Inputs are animation Data
		if ( hasAlphaInputs( mfnShaderNode ) )  {
			if ( !stateSet.get() )  stateSet = new osg::StateSet() ;
			stateSet -> setMode( GL_BLEND , true ) ;
			stateSet -> setRenderingHint( osg::StateSet::TRANSPARENT_BIN ) ;
		}
	}


	// search for any texture connected to color or outColor 
	MItDependencyGraph mItDG( mPlugColor ,  MFn::kFileTexture ,
									 MItDependencyGraph::kUpstream , 
									 MItDependencyGraph::kBreadthFirst ,
									 MItDependencyGraph::kNodeLevel , 
									 & mStatus ) ;

	if ( mStatus == MS::kFailure )  {
		logFile << "WARNING : mItDG( mPlugColor , MFn::kFileTexture , ... ) : mStatus == MS::kFailure at " << mPlugColor.name() << std::endl ;
		//return stateSet ;
	}

	mItDG.disablePruningOnFilter() ;


	// If no texture file node was found, just continue.
	bool hasTexture = true ;
	if ( mItDG.isDone() )  {
		logFile << "No texture file node was found" << std::endl ;
		hasTexture = false ;
	}

	// Print out the texture node name and texture file
	// that mItDG references.
	MString textureName = "" ;

	if ( hasTexture )  {
		osgColor.r() = diffuse ;
		osgColor.g() = diffuse ;
		osgColor.b() = diffuse ;
		MObject mTexture = mItDG.thisNode() ;
		MPlug mPlugFilename = MFnDependencyNode( mTexture ).findPlug( "fileTextureName" , true , & mStatus ) ;
		logFile << "TextureMod Color and Alpha : " << osgColor[0] << " , " << osgColor[1] << " , " << osgColor[2] << " , " << osgColor[3]<< std::endl ;

		if ( mStatus != MS::kSuccess )  {
			logFile << "WARNING : Plug \"fileTextureName\" not found !" << std::endl ;
		}
		else  {
			mPlugFilename.getValue( textureName ) ;
		}
	}

	if ( textureName.length() )   {
		logFile << "Texture file name : " << textureName.asChar() << std::endl ;
		if ( !stateSet.get() )  stateSet = new osg::StateSet() ;
		getStateSetTexture( stateSet , textureName.asChar() , texRepeat ) ;
	}



	// Phong Material
	if ( mfnShaderNode.typeName() == MString( "phong" ) )  {
		osg::ref_ptr< osg::Material > osgMaterial( new osg::Material ) ;

		osgMaterial -> setDiffuse( osg::Material::FRONT_AND_BACK , osgColor ) ;
		osgMaterial -> setAlpha( osg::Material::FRONT_AND_BACK , osgColor.a() ) ;

		getMaterialAttribute( mfnShaderNode , "ambientColor" ) ;
		osgMaterial -> setAmbient( osg::Material::FRONT_AND_BACK , osgColor ) ;

		getMaterialAttribute( mfnShaderNode , "specularColor" ) ;
		osgMaterial -> setSpecular( osg::Material::FRONT_AND_BACK , osgColor ) ;

		getMaterialAttribute( mfnShaderNode , "incandescence" ) ;
		osgMaterial -> setEmission( osg::Material::FRONT_AND_BACK , osgColor ) ;

		osgMaterial -> setShininess ( osg::Material::FRONT_AND_BACK , getMaterialAttribute( mfnShaderNode , "cosinePower" ) ) ;	//  / 100.0 * 128.0 

		if ( isAnimated )
			expAnim.mapInputConnections( shaderNode , osgMaterial ) ;

		if ( !stateSet.get() )  stateSet = new osg::StateSet() ;
		stateSet -> setAttributeAndModes( osgMaterial ) ;
		//stateSet -> setMode( GL_LIGHTING , osg::StateAttribute::ON ) ;

	}

	// Surface Shader Material
	else if ( mfnShaderNode.typeName() == MString( "surfaceShader" ) )  {
		if ( !stateSet.get() )  stateSet = new osg::StateSet() ;
		if ( isAnimated )  {
			osg::ref_ptr< osg::Material > osgMaterial( new osg::Material ) ;
			osgMaterial -> setColorMode( osg::Material::EMISSION ) ;
			osgMaterial -> setDiffuse( osg::Material::FRONT_AND_BACK , osgColor ) ;
			osgMaterial -> setAmbient( osg::Material::FRONT_AND_BACK , osg::Vec4( 0.f , 0.f , 0.f , 1.f ) ) ;

			expAnim.mapInputConnections( shaderNode , osgMaterial ) ;
			stateSet -> setAttributeAndModes( osgMaterial ) ;
		}

		else  stateSet -> setMode( GL_LIGHTING , osg::StateAttribute::OFF ) ;
	}

	// Lambert Material if Animated
	else if ( mfnShaderNode.typeName() == MString( "lambert" ) && isAnimated )  {
		if ( !stateSet.get() )  stateSet = new osg::StateSet() ;

		osg::ref_ptr< osg::Material > osgMaterial( new osg::Material ) ;
		//osgMaterial -> setColorMode( osg::Material::DIFFUSE  ) ;
		osgMaterial -> setDiffuse( osg::Material::FRONT_AND_BACK , osgColor ) ;

		osgColor.a() = 1.f ;
		getMaterialAttribute( mfnShaderNode , "ambientColor" ) ;
		osgMaterial -> setAmbient( osg::Material::FRONT_AND_BACK , osgColor ) ;

		expAnim.mapInputConnections( shaderNode , osgMaterial ) ;
		stateSet -> setAttributeAndModes( osgMaterial ) ;

	}

	if ( stateSet.get() )
		mapShader.insert( make_pair( shaderName , stateSet ) ) ;

	return stateSet ;

}



///////////////////////////////////////////////////////////////////////////////////////////////////
// - Has transparency or outTransparency Input Connections ----------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
bool Mesh::hasAlphaInputs( MFnDependencyNode & mfnShaderNode )
{
	bool hasInputs = false ;
	MPlugArray mPlugInputArray ;

	MPlug mPlug = mfnShaderNode.findPlug( "transparency" , true , & mStatus ) ;
	if ( mStatus != MS::kSuccess )  {
		mPlug = mfnShaderNode.findPlug( "outTransparency" , true , & mStatus ) ;
		if ( mStatus != MS::kSuccess )  {
			return false ;
		}
	}

	// Check for parent plugs
	mPlug.connectedTo( mPlugInputArray , true , false ) ;
	hasInputs = hasInputs || ( mPlugInputArray.length() > 0 ) ;

	// If the Attribute is compound ( currently each is compound, e.g. tx,ty,tz ) check for child plugs
	if ( mPlug.isCompound() )  {
		for( unsigned int cp = 0 ; cp < mPlug.numChildren() ; ++cp )  {							// cp = childPlug
			mPlug.child( cp ).connectedTo( mPlugInputArray , true , false ) ;	
			hasInputs = hasInputs || ( mPlugInputArray.length() > 0 ) ;
		}
	}

	else return false ;

	return hasInputs ;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// - Get Maya Material Attribute ------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
float Mesh::getMaterialAttribute( const MFnDependencyNode & mfnShaderNode , const MString & attr /*, MPlug & mPlug*/ )
{
	return getMaterialAttribute( mfnShaderNode.findPlug( attr , true , & mStatus ) ) ;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// - Get Maya Material Attribute ------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
float Mesh::getMaterialAttribute( const MPlug & mPlugShader )
{
	if ( mPlugShader.isCompound() )  {
		mPlugShader.child( 0 ).getValue( osgColor.r() ) ;
		mPlugShader.child( 1 ).getValue( osgColor.g() ) ;
		mPlugShader.child( 2 ).getValue( osgColor.b() ) ;
	}

	else 
		return mPlugShader.asFloat() ;

	return 0.0f ;

}



///////////////////////////////////////////////////////////////////////////////////////////////////
// - Get Texture for StateSet ---------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
void Mesh::getStateSetTexture( osg::ref_ptr< osg::StateSet > stateSet , const std::string & texName , bool texRepeat )
{
	logFile << "getStateSetTexture" << std::endl ;

	osg::ref_ptr< osg::Texture2D > texture = createTexture( texName , texRepeat ) ;

	if ( texture.valid() )  {
		stateSet -> setTextureAttributeAndModes( 0 , texture.get() , osg::StateAttribute::ON ) ;

		// as in Maya, texture replaces base osgColor
		osg::ref_ptr< osg::TexEnv > texenv = new osg::TexEnv ;
		texenv -> setMode( osg::TexEnv::MODULATE ) ;

		stateSet -> setTextureAttribute( 0 , texenv ) ;

	}

	logFile << "Done.." << std::endl ;

}



///////////////////////////////////////////////////////////////////////////////////////////////////
// - Create Texture -------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr< osg::Texture2D >Mesh::createTexture( const std::string & texName , bool texRepeat )
{
	// To do
	// Error Checking at name = strrchr, check against NULL ( File not found )
	logFile << "createTexture" << std::endl ;

	osg::ref_ptr< osg::Texture2D > texture ;

	// file name without full path
	std::string name = "" ;

	// win32 file name
	if( name.find(":") != std::string::npos && name.find( "\\" ) )
		name = strrchr( texName.c_str() , '\\' ) + 1 ; // just the file name

	else
		name = strrchr( texName.c_str() , '/') + 1 ; // just the file name


	logFile << "Begin texture file " << texName << " processing : " << name << std::endl ;

	// load image
	osg::ref_ptr< osg::Image > image = new osg::Image;
	image -> setFileName( name ) ;

	/*
	image->setFileName(e_name);
	string texture_extension = short_file_name.substr( short_file_name.length() - 3 , 3 ) ;
	*/

	// create texture
	texture = new osg::Texture2D ;
	texture -> setImage( image ) ;

	if ( texRepeat )  {
		texture -> setWrap( osg::Texture::WRAP_S , osg::Texture::REPEAT ) ;
		texture -> setWrap( osg::Texture::WRAP_T , osg::Texture::REPEAT ) ;
		texture -> setWrap( osg::Texture::WRAP_R , osg::Texture::REPEAT ) ;
	}

	texture -> setUnRefImageDataAfterApply( true ) ;
	return texture ;
}

#endif
