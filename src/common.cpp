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

#include "common.h"
#include "maya2osg.h"
#include "osg2maya.h"

#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MFnPlugin.h>

#include <osg/Version>


/**
 *
 */
MStatus initializePlugin( MObject _obj )
{
	std::cout << "Loading plug-in Maya2OSG. " << VENDOR << " " << VERSION << " (OSG " << osgGetVersion() << ")" << std::endl;
	MFnPlugin	plugin( _obj, VENDOR, VERSION );
	MStatus		stat;
	stat = plugin.registerCommand( "maya2osg",
	                                maya2osg::creator );
	if ( MCheckStatus(stat, "registerCommand - maya2osg") ) {
		return stat;
	}
	stat = plugin.registerCommand( "osg2maya",
	                                osg2maya::creator );
	if ( MCheckStatus(stat, "registerCommand - osg2maya") ) {
		return stat;
	}
	return MStatus::kSuccess;
}


/**
 *
 */
MStatus uninitializePlugin( MObject _obj )
{
	std::cout << "Unloading plug-in Maya2OSG. " << VENDOR << " " << VERSION << " (OSG " << osgGetVersion() << ")" << std::endl;
	MFnPlugin	plugin( _obj );
	MStatus		stat;
	stat = plugin.deregisterCommand( "maya2osg" );
	if ( MCheckStatus(stat, "deregisterCommand - maya2osg") ) {
		return stat;
	}
	stat = plugin.deregisterCommand( "osg2maya" );
	if ( MCheckStatus(stat, "deregisterCommand - osg2maya") ) {
		return stat;
	}
	return MStatus::kSuccess;
}


/**
 *
 */
bool MCheckStatus(MStatus &st, const std::string &msg)
{
	if( st.statusCode() != MS::kSuccess )
	{
		std::cout << msg << " : " << st.errorString().asChar() << std::endl;
		return true;
	}
	return false;
}
