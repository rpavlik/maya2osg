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

#include "glslutils.h"

#include <osg/CullFace>


std::string GLSLUtils::backFaceCulling( osg::StateSet &sset )
{
	std::string glsl_code =
"    vec3 nnormal;\n";

    bool opposite = false;
    osg::StateAttribute *sa = sset.getAttribute( osg::StateAttribute::CULLFACE );
    if ( sa ) {
        osg::CullFace *cf = dynamic_cast<osg::CullFace*>(sa);
        if ( cf ) {
            // If culling front facing polygons, we consider it the opposite direction
            opposite = cf->getMode() == osg::CullFace::FRONT;
        }
    }

    // Check if the StateSet has backface culling enabled 
    // and add code to the shader to perform it 
    if ( sset.getMode(GL_CULL_FACE) == osg::StateAttribute::ON ) {
        if ( opposite ) {
            glsl_code += 
"    if ( gl_FrontFacing )\n"
"        discard;\n"
"    else\n"
"        nnormal = normalize(-normal);\n";
        }
        else {
            glsl_code += 
"    if ( !gl_FrontFacing )\n"
"        discard;\n"
"    else\n"
"        nnormal = normalize(normal);\n";
        }
        glsl_code += 
"\n";
    }
    else {
        if ( opposite ) {
            glsl_code += 
"    if ( !gl_FrontFacing )\n";
        }
        else {
            glsl_code += 
"    if ( gl_FrontFacing )\n";
        }
        glsl_code += 
"        nnormal = normalize(normal);\n"
"    else\n"
"        nnormal = normalize(-normal);\n"
"\n";
    }

	return glsl_code;
}
