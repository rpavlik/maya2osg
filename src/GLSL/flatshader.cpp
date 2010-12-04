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

#include "flatshader.h"
#include "shadingnetwork.h"
#include "glslutils.h"
#include "../common.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

#include <sstream>

/**
 *  Constructor
 */
FlatShader::FlatShader( const MObject &shading_node, ShadingNetwork &shading_network ) :
    SurfaceShader( shading_node, shading_network ) 
{
}


/**
 *  Get the GLSL Codeblock for a plug
 */
ShadingNode::CodeBlock FlatShader::getCodeBlock( const std::string &plug_name )
{
    // Build the GLSL code ...
    CodeBlock code_block;

    std::string variable_name;
    MFnDependencyNode dn(_mayaShadingNode);

    // Check plug name and avoid duplicating code

    if ( plug_name == "fragmentOutput" ||
         plug_name == "outColor" || 
         plug_name == "outTransparency" ) 
    {
        variable_name = "sn_flat_" + std::string(dn.name().asChar()) + "_output";
        // Both color and transparency are stored in "color" variable.
        // if it is already declared, we omit all the GLSL code
        if ( !variableIsAvailable(variable_name) ) {

            // Supported flat shader (SurfaceShader) input channels

            // Color
            Plug plug_color = getPlug("outColor");
            // Transparency
            Plug plug_transparency = getPlug("outTransparency");

            code_block.declarations = plug_color.codeBlock.declarations +
                                      plug_transparency.codeBlock.declarations;

            code_block.functions = plug_color.codeBlock.functions +
                                   plug_transparency.codeBlock.functions;

            code_block.computeCode = plug_color.codeBlock.computeCode +
                                     plug_transparency.codeBlock.computeCode +
                                     GLSLUtils::backFaceCullingWithoutNormals(_shadingNetwork.getStateSet());

            code_block.computeCode +=
"    vec4 " + variable_name + " = vec4( " + getPlugValue(plug_color) + ", 1.0 - " + getPlugValue(plug_transparency) + " );\n";

            _computedVariables.insert(variable_name);
        }
        // Access code
        if ( plug_name == "fragmentOutput" ) {  // special code for fragment output (RGBA)
            code_block.accessCode = variable_name;
        }
        else if ( plug_name == "outColor" ) {
            code_block.accessCode = variable_name + ".rgb";
        }
        else if ( plug_name == "outTransparency" ) {
            code_block.accessCode = "(1.0 - " + variable_name + ".a)";
        }
    }
    else {
        std::cerr << "WARNING: Unsupported plug " << plug_name << " in Flat shader " << dn.name().asChar() << std::endl;
        // throw exception ??? !!! FIXME!!!
    }

    return code_block;
}


/**
 *  Get the vertex shader GLSL source code
 */
std::string FlatShader::getVertexShaderSrc()
{
    std::string shader_src =
"// Exported with Maya2OSG " VERSION "\n"
"// http://maya2osg.sourceforge.net\n"
"#version 120\n"
"\n"
"void main() {\n"
"	// Transform vertex from object space to clip space\n"
"	gl_Position = ftransform();\n"
"\n";

    for ( std::set<int>::const_iterator i = _shadingNetwork.getTexturingConfig().getTCSetsUnitsUsed().begin() ; 
            i != _shadingNetwork.getTexturingConfig().getTCSetsUnitsUsed().end() ; i++ ) 
    {
		std::stringstream tc_set;
		tc_set << *i;
		shader_src += 
"	gl_TexCoord[" + tc_set.str() + "] = gl_TextureMatrix[" + 
				tc_set.str() + "] * gl_MultiTexCoord" + tc_set.str() + ";\n"
;
	}
    shader_src +=
"}\n";

	return shader_src;
}


/**
 *  Get the fragment shader GLSL source code
 */
std::string FlatShader::getFragmentShaderSrc()
{
    SurfaceShader::CodeBlock cb_fragment_output = getCodeBlock("fragmentOutput");
    bool bump_map = hasBumpMap();

    std::string shader_src;
    shader_src =
"// Exported with Maya2OSG " VERSION "\n"
"// http://maya2osg.sourceforge.net\n"
"#version 120\n"
"\n";
    // Declarations
    shader_src += cb_fragment_output.declarations;
    // Functions
    shader_src += cb_fragment_output.functions;

    // Main
    shader_src +=
"void main()\n"
"{\n"
;
    // inline code
    shader_src += cb_fragment_output.computeCode;

    // variable names
    shader_src += 
"    gl_FragColor = " + cb_fragment_output.accessCode + ";\n"
"}\n"
;
    return shader_src;
}
