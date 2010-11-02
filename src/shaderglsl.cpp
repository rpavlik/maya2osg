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
#include "shaderglsl.h"
#include "glsl/lambert.h"
#include "shader.h"

#include <osg/BlendFunc>


/**
 *	Configure the shaders for the StateSet of the ShadingEngine/ShadingGroup object
 */
void ShaderGLSL::exporta(const MObject &shading_engine, TexturingConfig &texturing_config,
						osg::StateSet &state_set)
{
	MObject surface_shader;
	Shader::getSurfaceShader( shading_engine, surface_shader );

	if ( surface_shader.hasFn(MFn::kLambert) ) {
        Lambert::configureGLSLProgram( surface_shader, texturing_config, state_set );
	}
	//else if ...
	// TO-DO : rest (non Lambert-derived) of Maya shaders *****  FIXME!!!
}
