#ifndef _UTILITY_H_
#define _UTILITY_H_


// ----- Maya -----------------------------------------------------------------------
#include <maya/MPoint.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MFloatPoint.h>


// ----- OSG ------------------------------------------------------------------------
#include <osg/Vec3>
#include <osg/Matrixd>
#include <osg/Matrixf>


// CHECK if distance from BASE is in TOLLERANCE
static inline bool inTollerance( double base , double match , double tollerance )  {
	return ( base - tollerance <= match ) && ( base + tollerance >= match ) ;
}

//------------------------------------------------------------------------------
static inline void assign( MFloatPoint & mFloatPoint , const osg::Vec3 & osgVec3 )  {
	mFloatPoint[ 0 ] = osgVec3[ 0 ] ;
	mFloatPoint[ 1 ] = osgVec3[ 1 ] ;
	mFloatPoint[ 2 ] = osgVec3[ 2 ] ;
}

//------------------------------------------------------------------------------
static inline void assign( osg::Vec3 & osgVec3 , const MPoint & mPoint )  {
	osgVec3[ 0 ] = mPoint[ 0 ] ;
	osgVec3[ 1 ] = mPoint[ 1 ] ;
	osgVec3[ 2 ] = mPoint[ 2 ] ;
}

//------------------------------------------------------------------------------
static inline void assign( MVector & mVector , const osg::Vec3 & osgVec3 )  {
	mVector[ 0 ] = osgVec3[ 0 ] ;
	mVector[ 1 ] = osgVec3[ 1 ] ;
	mVector[ 2 ] = osgVec3[ 2 ] ;
}

//------------------------------------------------------------------------------
static inline void assign( osg::Vec3 & osgVec3 , const MVector & mVector )  {
	osgVec3[ 0 ] = mVector[ 0 ] ;
	osgVec3[ 1 ] = mVector[ 1 ] ;
	osgVec3[ 2 ] = mVector[ 2 ] ;
}

//------------------------------------------------------------------------------
static inline void assign( osg::Matrixf & osgMatrix , const MMatrix & mMatrix )  {
	osgMatrix = osg::Matrixf(
		mMatrix( 0 , 0 ) , mMatrix( 0 , 1 ) , mMatrix( 0 , 2 ) , mMatrix( 0 , 3 ) ,
		mMatrix( 1 , 0 ) , mMatrix( 1 , 1 ) , mMatrix( 1 , 2 ) , mMatrix( 1 , 3 ) ,
		mMatrix( 2 , 0 ) , mMatrix( 2 , 1 ) , mMatrix( 2 , 2 ) , mMatrix( 2 , 3 ) ,
		mMatrix( 3 , 0 ) , mMatrix( 3 , 1 ) , mMatrix( 3 , 2 ) , mMatrix( 3 , 3 ) ) ;
}

//------------------------------------------------------------------------------
static inline void assign( osg::Matrixd & osgMatrix , const MMatrix & mMatrix )  {
	osgMatrix = osg::Matrixd(
		mMatrix( 0 , 0 ) , mMatrix( 0 , 1 ) , mMatrix( 0 , 2 ) , mMatrix( 0 , 3 ) ,
		mMatrix( 1 , 0 ) , mMatrix( 1 , 1 ) , mMatrix( 1 , 2 ) , mMatrix( 1 , 3 ) ,
		mMatrix( 2 , 0 ) , mMatrix( 2 , 1 ) , mMatrix( 2 , 2 ) , mMatrix( 2 , 3 ) ,
		mMatrix( 3 , 0 ) , mMatrix( 3 , 1 ) , mMatrix( 3 , 2 ) , mMatrix( 3 , 3 ) ) ;
}
#endif	// _UTILITY_H_