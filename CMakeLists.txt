#
#    Maya2OSG - A toolkit for exporting Maya scenes to OpenSceneGraph
#    Copyright (C) 2010 Javier Taibo <javier.taibo@gmail.com>
#
#    This file is part of Maya2OSG.
#
#    Maya2OSG is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Maya2OSG is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Maya2OSG.  If not, see <http://www.gnu.org/licenses/>.
#

# Thanks to Diego/ScEngMan for some hints on the creation of this CMake script
#
# http://area.autodesk.com/forum/autodesk-maya/sdk/build-maya-plug-ins-using-cmake-40a-howto41
# http://crackart.org/wiki/HowTo/CMakeForMaya

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Maya2OSG)

SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 4)
SET(VERSION_RELEASE 2)
SET(VERSION_PATCH c)

# Set the Maya version and architecture (default values)
SET(MAYA_VERSION 2011 CACHE STRING "Maya Version")
SET(MAYA_ARCH x64 CACHE STRING "HW Architecture")

IF(WIN32)
	SET( MAYA_USER_DIR $ENV{USERPROFILE}/Documents/maya CACHE PATH "Maya home directory" )
	SET( MAYA_ROOT $ENV{PROGRAMFILES}/Autodesk/Maya${MAYA_VERSION} )
	SET( MAYA_SDK_DEFINITIONS "_AFXDLL;_MBCS;NT_PLUGIN;REQUIRE_IOSTREAM" )
ELSE(WIN32)
	SET( MAYA_USER_DIR $ENV{HOME}/maya CACHE PATH "Maya home directory" )
	SET( MAYA_ROOT /usr/autodesk/maya${MAYA_VERSION}-${MAYA_ARCH} )
ENDIF(WIN32)

SET( MAYA_HEADERS_DIR ${MAYA_ROOT}/include )
SET( MAYA_LIBRARY_DIR ${MAYA_ROOT}/lib )
IF (EXISTS $ENV{MAYA_PLUG_IN_PATH})
	SET ( MAYA_PLUGINS_DIR $ENV{MAYA_PLUG_IN_PATH} CACHE PATH "Maya plug-in installation directory" )
ELSE (EXISTS $ENV{MAYA_PLUG_IN_PATH})
	SET( MAYA_PLUGINS_DIR ${MAYA_ROOT}/bin/plug-ins CACHE PATH "Maya plug-in installation directory" )
ENDIF (EXISTS $ENV{MAYA_PLUG_IN_PATH})

FIND_PACKAGE(OpenGL)
FIND_PACKAGE(OpenThreads)
FIND_PACKAGE(osg)
FIND_PACKAGE(osgViewer)
FIND_PACKAGE(osgDB)
FIND_PACKAGE(osgAnimation)
FIND_PACKAGE(osgShadow)
FIND_PACKAGE(osgParticle)

MACRO( CONFIG_PROJECT PROJNAME LIBNAME )
	INCLUDE_DIRECTORIES( ${${LIBNAME}_INCLUDE_DIR} )
    TARGET_LINK_LIBRARIES( ${PROJNAME} ${${LIBNAME}_LIBRARY} )
ENDMACRO()

INCLUDE_DIRECTORIES ( ${PUBLIC_INCLUDE_DIRS} ${MAYA_HEADERS_DIR} )
LINK_DIRECTORIES ( ${PUBLIC_LIBRARY_DIRS} ${MAYA_LIBRARY_DIR} )

SET ( LOCAL_WARNING_FLAGS /W3 )
SET ( LOCAL_RTTI_FLAGS /GR )

SET(CMAKE_INSTALL_PREFIX ${MAYA_PLUGINS_DIR})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

SET ( TARGET "maya2osg" )

SET ( SOURCE_FILES 
		src/animation.cpp
		src/animation.h
		src/camera.cpp
		src/camera.h
		src/cameraanimation.cpp
		src/cameraanimation.h
		src/common.cpp
		src/common.h
		src/config.cpp
		src/config.h
		src/dagnode.cpp
		src/dagnode.h
		src/directionallight.cpp
		src/directionallight.h
		src/field.cpp
		src/field.h
		src/group.cpp
		src/group.h
		src/importervisitor.cpp
		src/importervisitor.h
		src/lights.cpp
		src/lights.h
		src/maya2osg.cpp
		src/maya2osg.h
		src/mesh.cpp
		src/mesh.h
		src/nparticle.cpp
		src/nparticle.h
		src/osg2maya.cpp
		src/osg2maya.h
		src/osgfiletranslator.cpp
		src/osgfiletranslator.h
		src/osgwrite.cpp
		src/osgwrite.h
		src/particle.cpp
		src/particle.h
		src/pointemitter.cpp
		src/pointemitter.h
		src/pointlight.cpp
		src/pointlight.h
		src/proxymaterials.cpp
		src/proxymaterials.h
		src/shader.cpp
		src/shader.h
		src/shadows.cpp
		src/shadows.h
		src/spotlight.cpp
		src/spotlight.h
		src/texture.cpp
		src/texture.h
		src/transform.cpp
		src/transform.h
		src/utility.h
		src/GLSL/bump2d.cpp
		src/GLSL/bump2d.h
		src/GLSL/bump2normal.cpp
		src/GLSL/bump2normal.h
		src/GLSL/filetexture.cpp
		src/GLSL/filetexture.h
		src/GLSL/flatshader.cpp
		src/GLSL/flatshader.h
		src/GLSL/glslutils.cpp
		src/GLSL/glslutils.h
		src/GLSL/lambert.cpp
		src/GLSL/lambert.h
		src/GLSL/phong.cpp
		src/GLSL/phong.h
		src/GLSL/shadingnetwork.cpp
		src/GLSL/shadingnetwork.h
		src/GLSL/shadingnode.cpp
		src/GLSL/shadingnode.h
		src/GLSL/shadingnodefactory.cpp
		src/GLSL/shadingnodefactory.h
		src/GLSL/surfaceshader.cpp
		src/GLSL/surfaceshader.h
		src/GLSL/texturingconfig.cpp
		src/GLSL/texturingconfig.h
	)

SET( LIBRARIES "Foundation.lib" "OpenMaya.lib" "OpenMayaAnim.lib" "OpenMayaFX.lib" "OpenMayaRender.lib" )

ADD_LIBRARY ( ${TARGET} SHARED ${SOURCE_FILES} )
TARGET_LINK_LIBRARIES( ${TARGET} ${LIBRARIES} )

CONFIG_PROJECT( ${TARGET} OPENGL)
CONFIG_PROJECT( ${TARGET} OPENTHREADS)
CONFIG_PROJECT( ${TARGET} OSG)
CONFIG_PROJECT( ${TARGET} OSGVIEWER)
CONFIG_PROJECT( ${TARGET} OSGDB)
CONFIG_PROJECT( ${TARGET} OSGANIMATION)
CONFIG_PROJECT( ${TARGET} OSGSHADOW)
CONFIG_PROJECT( ${TARGET} OSGPARTICLE)

SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES COMPILE_DEFINITIONS "${MAYA_SDK_DEFINITIONS}" )
SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES OUTPUT_NAME "maya2osg" )
SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES SUFFIX ".mll" )
SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES CLEAN_DIRECT_OUTPUT 1 )
SET_TARGET_PROPERTIES( ${TARGET} PROPERTIES LINK_FLAGS "/export:initializePlugin /export:uninitializePlugin" )

INSTALL( TARGETS ${TARGET} RUNTIME DESTINATION ${MAYA_PLUGINS_DIR} )
INSTALL( DIRECTORY ${CMAKE_SOURCE_DIR}/scripts DESTINATION ${MAYA_USER_DIR}/${MAYA_VERSION} PATTERN ".svn" EXCLUDE PATTERN "*.mel" )
INSTALL( DIRECTORY ${CMAKE_SOURCE_DIR}/prefs/icons DESTINATION ${MAYA_USER_DIR}/${MAYA_VERSION}/prefs PATTERN ".svn" EXCLUDE PATTERN "*.bmp" )
INSTALL( DIRECTORY ${CMAKE_SOURCE_DIR}/prefs/shelves DESTINATION ${MAYA_USER_DIR}/${MAYA_VERSION}/prefs PATTERN ".svn" EXCLUDE PATTERN "*.mel" )
