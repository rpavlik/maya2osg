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
#include "bump2d.h"
#include "../common.h"

#include <maya/MFnDependencyNode.h>

#define RUN_TIME_BUMP_DEPTH // *** TO-DO: document this!

/**
 *  Constructor
 */
Bump2D::Bump2D( const MObject &shading_node, ShadingNetwork &shading_network ) :
    ShadingNode( shading_node, shading_network ) 
{
}


/**
 *  Get the GLSL Codeblock for a plug
 */
ShadingNode::CodeBlock Bump2D::getCodeBlock( const std::string &plug_name )
{
    CodeBlock code_block;

    std::string variable_name;
    MFnDependencyNode dn(_mayaShadingNode);
    std::string maya_bump_name = dn.name().asChar();
    hygienizeName( maya_bump_name );

    // Check the plug name. Supported ones: outNormal

    if ( plug_name == "outNormal" )
    {
        variable_name = "sn_bump2d_" + maya_bump_name + "_outNormal";

        // If variable not available, we compute them
        if ( ! variableIsAvailable(variable_name) ) {

            // Bump value
            Plug plug_bump_value = getPlug("bumpValue");
#ifdef RUN_TIME_BUMP_DEPTH
            // Bump depth
            Plug plug_bump_depth = getPlug("bumpDepth");
#endif

            code_block.declarations = plug_bump_value.codeBlock.declarations
#ifdef RUN_TIME_BUMP_DEPTH
                                    + plug_bump_depth.codeBlock.declarations
#endif
                                    ;

            code_block.functions = plug_bump_value.codeBlock.functions
#ifdef RUN_TIME_BUMP_DEPTH
                                 + plug_bump_depth.codeBlock.functions
#endif
                                 ;

#ifdef RUN_TIME_BUMP_DEPTH
            std::string unperturbed = "sn_bump2d_" + maya_bump_name + "_unperturbed";
            std::string delta = "sn_bump2d_" + maya_bump_name + "_delta";
            code_block.computeCode = plug_bump_value.codeBlock.computeCode
                                   + plug_bump_depth.codeBlock.computeCode
                                   + "    vec3 " + unperturbed + " = vec3(0.0, 0.0, 1.0);\n"
                                   + "    vec3 " + delta + " = "
                                   + "normalize(" + getPlugValue(plug_bump_value) + " - 0.5) - " + unperturbed + ";\n"
                                   + "    vec3 " + variable_name + " = " + unperturbed + " + " + delta + " * "
                                   + getPlugValue(plug_bump_depth) + ";\n"
                                   ;
#else
            code_block.computeCode = plug_bump_value.codeBlock.computeCode
                                   + "    vec3 " + variable_name + " = ( " 
//                                   + "normalize(2.0 * (" + getPlugValue(plug_bump_value) + " - 0.5))"
                                   + "normalize(" + getPlugValue(plug_bump_value) + " - 0.5)"
                                   + " );\n";
#endif

            _computedVariables.insert(variable_name);
        }

        // Access code
        code_block.accessCode = variable_name;
    }
    else {
        std::cerr << "WARNING: Unsupported plug " << plug_name << " in Bump2D shading node " << dn.name().asChar() << std::endl;
        // throw exception ??? !!! FIXME!!!
    }

    return code_block;
}
