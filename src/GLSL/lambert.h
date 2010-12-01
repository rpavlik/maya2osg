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
#ifndef _LAMBERT_H_
#define _LAMBERT_H_

#include "surfaceshader.h"

#include <string>


class Lambert : public SurfaceShader {

public:

    /// Constructor
    Lambert( const MObject &shading_node, ShadingNetwork &shading_network );

    /// Get the GLSL Codeblock for a plug
    virtual CodeBlock getCodeBlock( const std::string &plug_name );

private:

	std::string getDirectionalLightFunction();
	std::string getPointLightFunction();
	std::string getSpotLightFunction();

};

#endif //_LAMBERT_H_
