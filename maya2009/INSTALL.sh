#!/bin/sh

#set -v

echo " *** Edit INSTALL.sh to customize paths *** "

MAYA_VERSION=2009-x64
echo "MAYA_VERSION=${MAYA_VERSION}"

MAYA_DIR=${HOME}/maya/${MAYA_VERSION}
echo "MAYA_DIR=${MAYA_DIR}"

if [ "${MAYA_PLUG_IN_PATH}" != "" ] ; then
	MAYA_BIN_PLUGINS_DIR=${MAYA_PLUG_IN_PATH}
else
	MAYA_BIN_PLUGINS_DIR=${MAYA_DIR}/bin/plug-ins
fi
echo "Installing maya2osg binary to ${MAYA_BIN_PLUGINS_DIR} ..."
mkdir -p  ${MAYA_BIN_PLUGINS_DIR}
cp bin/maya2osg.so  ${MAYA_BIN_PLUGINS_DIR}

MAYA_SCRIPTS_DIR=${MAYA_DIR}/scripts
echo "Installing maya2osg scripts to ${MAYA_SCRIPTS_DIR} ..."
mkdir -p ${MAYA_SCRIPTS_DIR}
cp ../scripts/*.mel ${MAYA_SCRIPTS_DIR}

MAYA_PREFS_ICONS_DIR=${MAYA_DIR}/prefs/icons
echo "Installing maya2osg icons to ${MAYA_PREFS_ICONS_DIR} ..."
mkdir -p ${MAYA_PREFS_ICONS_DIR}
cp ../prefs/icons/*.bmp ${MAYA_PREFS_ICONS_DIR}

MAYA_PREFS_SHELVES_DIR=${MAYA_DIR}/prefs/shelves
echo "Installing maya2osg shelves to ${MAYA_PREFS_SHELVES_DIR} ..."
mkdir -p ${MAYA_PREFS_SHELVES_DIR}
cp ../prefs/shelves/*.mel ${MAYA_PREFS_SHELVES_DIR}
