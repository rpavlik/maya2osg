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
#ifndef _TEXTURINGCONFIG_H_
#define _TEXTURINGCONFIG_H_

#include <map>
#include <set>
#include <string>

/**
 *  This class stores the texturing configuration:
 *
 *  Relations between Maya texture nodes, OpenGL 
 *  texturing units and texture coordinate sets
 */
class TexturingConfig {

public:

    /// Constructor
    TexturingConfig();

    /// Bind a Maya texture with a TC set
    void setTCSet( const std::string &maya_name, int tc_set );

    /// Query the TC set bound to a texture
    int getTCSet( const std::string &maya_name );

    /// Get the set of TC sets used (texturing units)
    const std::set<int> &getTCSetsUnitsUsed();

    /// Get the texturing unit a texture is bound to
    int getTextureUnit( const std::string &maya_name );

private:

    /// Number of used texture units
    int _usedTextureUnits;

    /// Map that binds Maya texture names to TC sets
    std::map< std::string, int > _TCSetsMap;

    /// Map that binds Maya texture names to texturing units
    std::map< std::string, int > _texUnitMap;

    /// Set of texturing units used
    std::set<int> _TCSetsUnitsUsed;

};

#endif //_TEXTURINGCONFIG_H_
