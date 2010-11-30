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
#ifndef _GLSLUTILS_H_
#define _GLSLUTILS_H_

#include <osg/StateSet>

namespace GLSLUtils {

	/**
	 *	GLSL code that implements backface culling in the fragment shader
	 *	following the StateSet configuration
	 *
	 *	Apart from discarding culled polygons, it computes nnormal variable 
	 *	as the normalized normal oriented following the StateSet
	 *
	 *	@note vec3 normal must be declared prior to executing this code
	 *	@note vec3 nnormal is declared and initialized in this code
	 */
	std::string backFaceCulling( osg::StateSet &sset );

}

#endif //_GLSLUTILS_H_
