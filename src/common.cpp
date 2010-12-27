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
#include "osgfiletranslator.h"

#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MFnPlugin.h>

#include <osg/Version>

#include <algorithm>


/**
 *
 */
MStatus initializePlugin( MObject _obj )
{
	std::cout << "Loading Maya2OSG plug-in. " << VENDOR << " " << VERSION << " (" 
		<< osgGetLibraryName() << " " << osgGetVersion() << ")" << std::endl;
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
    stat = plugin.registerFileTranslator( "OSG", NULL, OSGFileTranslator::creator, "", "" );
	if ( MCheckStatus(stat, "registerFileTranslator - OSG") ) {
		return stat;
	}
	return MStatus::kSuccess;
}


/**
 *
 */
MStatus uninitializePlugin( MObject _obj )
{
	std::cout << "Unloading Maya2OSG plug-in. " << VENDOR << " " << VERSION << " (" 
		<< osgGetLibraryName() << " " << osgGetVersion() << ")" << std::endl;
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
	stat = plugin.deregisterFileTranslator( "OSG" );
	if ( MCheckStatus(stat, "deregisterFileTranslator - OSG") ) {
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


/**
 *  Remove dangerous characters from a Maya name.
 *
 *  Maya names use :, | and . to separate namespaces, DAG nodes and 
 *  attributes. These characters can cause problems in most uses,
 *  such as variable names or even file names.
 *
 *  @warning This substitution do not guarantee that name collisions
 *  won't occur, even though they probably won't.  TO-DO *** FIXME!!!
 */
void hygienizeName( std::string & variable_name )
{
    replace(variable_name.begin(), variable_name.end(), ':', '_');
    replace(variable_name.begin(), variable_name.end(), '|', '_');
    replace(variable_name.begin(), variable_name.end(), '.', '_');
}
