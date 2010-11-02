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
#include "texturingconfig.h"
#include <iostream>

/**
 *  Constructor
 */
TexturingConfig::TexturingConfig()
{
    _usedTextureUnits = 0;
}


/**
 *  Bind a Maya texture with a TC set
 */
void TexturingConfig::setTCSet( const std::string &maya_name, int tc_set )
{
    // Store the texturing unit for the texture coordinates for this texture
    _TCSetsMap[maya_name] = tc_set;
    // Register the texturing unit as being used for texture coordinates
//    _TCSetsUnitsUsed.insert( tc_set );    // *** we do it in getTextureUnit()
}


/**
 *  Get the texturing unit a texture is bound to
 */
int TexturingConfig::getTextureUnit( const std::string &maya_name )
{
    int unit;
    // Check if the texture is being used
    if ( _texUnitMap.find(maya_name) != _texUnitMap.end() ) {
        unit = _texUnitMap[maya_name];
    }
    else {
        unit = _usedTextureUnits++;
        // TO-DO: Check if there are enough OpenGL texturing units available
        // *** FIXME!
        _texUnitMap[maya_name] = unit;
        // Register the texturing unit as being used for texture coordinates
        _TCSetsUnitsUsed.insert( _TCSetsMap[maya_name] );
    }
    return unit;
}


/**
 *  Query the TC set bound to a texture
 */
int TexturingConfig::getTCSet( const std::string &maya_name )
{
    if ( _TCSetsMap.find( maya_name ) != _TCSetsMap.end() ) {
        return _TCSetsMap[maya_name];
    }
    else {
        return -1;
    }
}


/**
 *  Get the set of TC sets used (texturing units)
 */
const std::set<int> &TexturingConfig::getTCSetsUnitsUsed()
{
    return _TCSetsUnitsUsed;
}
