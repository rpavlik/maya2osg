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

#include "lambert.h"
#include "glsllighting.h"
#include "shadingnetwork.h"

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>

#include <osg/CullFace>


/**
 *  Constructor
 */
Lambert::Lambert( const MObject &shading_node, ShadingNetwork &shading_network ) :
    SurfaceShader( shading_node, shading_network ) 
{
}


/**
 *  Get the GLSL Codeblock for a plug
 */
ShadingNode::CodeBlock Lambert::getCodeBlock( const std::string &plug_name )
{
    // Build the GLSL code ...
    CodeBlock code_block;

    // WARNING. It is important to differentiate between the 
    // GLSL variable name and the return value
    // E.g. 
    //  The GLSL variable name 
    //
    //      vec4 sn_lambert_lambert1_color = ...
    //
    //  will serve to at least two output plugs with different GLSL code.
    //  WARNING! This code is what is called variableName in the CodeBlock structure
    //
    //      sn_lambert1_lambert1_color.rgb  -> plug "outColor"
    //      1.0 - sn_lambert1_lambert1_color.a    -> plug "outTransparency"
    //
    //  The variable name is stored in the map for the shading node plug
    //  The same variable name can be shared between plugs
    //  The access code for each plug is what is exported in the CodeBlock

    std::string variable_name;
    MFnDependencyNode dn(_mayaShadingNode);

    // Check plug name and avoid duplicating code

    if ( plug_name == "fragmentOutput" ||
         plug_name == "outColor" || 
         plug_name == "outTransparency" ) 
    {
        variable_name = "sn_lambert_" + std::string(dn.name().asChar()) + "_output";
        // Both color and transparency are stored in "color" variable.
        // if it is already declared, we omit all the GLSL code
        if ( !variableIsAvailable(variable_name) ) {

            // Supported Lambert input channels

            // Color (diffuse)
            Plug plug_color = getPlug("color");
            // Transparency
            Plug plug_transparency = getPlug("transparency");
            // Ambient Color
            Plug plug_ambient = getPlug("ambientColor");
            // Incandescence
            Plug plug_incandescence = getPlug("incandescence");
            // Bump map
            Plug plug_normal_camera = getPlug("normalCamera");
            // Diffuse (scalar)
            Plug plug_diffuse = getPlug("diffuse");

            code_block.declarations = plug_color.codeBlock.declarations +
                                      plug_transparency.codeBlock.declarations +
                                      plug_ambient.codeBlock.declarations +
                                      plug_incandescence.codeBlock.declarations +
                                      plug_normal_camera.codeBlock.declarations +
                                      plug_diffuse.codeBlock.declarations;

            code_block.functions = plug_color.codeBlock.functions +
                                   plug_transparency.codeBlock.functions +
                                   plug_ambient.codeBlock.functions +
                                   plug_incandescence.codeBlock.functions +
                                   plug_normal_camera.codeBlock.functions +
                                   plug_diffuse.codeBlock.functions +
                                   GLSLLighting::getDirectionalLightFunctionWithoutSpecular() +
                                   GLSLLighting::getPointLightFunctionWithoutSpecular() +
                                   GLSLLighting::getSpotLightFunctionWithoutSpecular();

            code_block.computeCode = plug_color.codeBlock.computeCode +
                                     plug_transparency.codeBlock.computeCode +
                                     plug_ambient.codeBlock.computeCode +
                                     plug_incandescence.codeBlock.computeCode +
                                     plug_normal_camera.codeBlock.computeCode +
                                     plug_diffuse.codeBlock.computeCode +
"\n" +
"    vec3 nnormal;\n";

            bool opposite = false;
            osg::StateAttribute *sa = _shadingNetwork.getStateSet().getAttribute( osg::StateAttribute::CULLFACE );
            if ( sa ) {
                osg::CullFace *cf = dynamic_cast<osg::CullFace*>(sa);
                if ( cf ) {
                    // If culling front facing polygons, we consider it the opposite direction
                    opposite = cf->getMode() == osg::CullFace::FRONT;
                }
            }

            // Check if the StateSet has backface culling enabled 
            // and add code to the shader to perform it 
            if ( _shadingNetwork.getStateSet().getMode(GL_CULL_FACE) == osg::StateAttribute::ON ) {
                if ( opposite ) {
                    code_block.computeCode += 
"    if ( gl_FrontFacing )\n"
"        discard;\n"
"    else\n"
"        nnormal = normalize(-normal);\n";
                }
                else {
                    code_block.computeCode += 
"    if ( !gl_FrontFacing )\n"
"        discard;\n"
"    else\n"
"        nnormal = normalize(normal);\n";
                }
                code_block.computeCode += 
"\n";
            }
            else {
                if ( opposite ) {
                    code_block.computeCode += 
"    if ( !gl_FrontFacing )\n";
                }
                else {
                    code_block.computeCode += 
"    if ( gl_FrontFacing )\n";
                }
                code_block.computeCode += 
"        nnormal = normalize(normal);\n"
"    else\n"
"        nnormal = normalize(-normal);\n"
"\n";
            }

	        // Bump mapping
            if ( hasBumpMap() ) {
                code_block.computeCode +=
"    nnormal = " + plug_normal_camera.codeBlock.accessCode + ";\n"
;
            }

            code_block.computeCode +=
"	vec3 eye;\n"
"	if (LocalViewer)\n"
"	    eye = -normalize(ecPosition3);\n"
"	else\n"
"	    eye = vec3(0.0, 0.0, 1.0);\n"
"\n"
"	// Clear the light intensity accumulators\n"
"	vec4 amb  = vec4(0.0);\n"
"	vec4 diff = vec4(0.0);\n"
"\n"
"	// Loop through enabled lights, compute contribution from each\n"
"	int i;\n"
"	for (i = 0; i < NumEnabledLights; i++)\n"
"	{\n"
"	    if (gl_LightSource[i].position.w == 0.0)\n"
"	        DirectionalLight(i, nnormal, amb, diff);\n"
"	    else if (gl_LightSource[i].spotCutoff == 180.0)\n"
"	        PointLight(i, eye, ecPosition3, nnormal, amb, diff);\n"
"	    else\n"
"	        SpotLight(i, eye, ecPosition3, nnormal, amb, diff);\n"
"	}\n"
"\n"
            // Channel : Diffuse color
"    vec3 diffuse_color = " + getPlugValue(plug_color) + " * " + getPlugValue(plug_diffuse) + ";\n"
            // Channel : Ambient color
"    vec3 ambient_color = " + getPlugValue(plug_ambient) + ";\n"
            // Channel : Emissive color
"    vec3 emissive_color = " + getPlugValue(plug_incandescence) + ";\n"
"\n";

    code_block.computeCode +=
"    vec4 " + variable_name + " = vec4( emissive_color +\n"
"                (amb.rgb + gl_LightModel.ambient.rgb) * ambient_color +\n"
"                diff.rgb * diffuse_color, 1.0 - " + getPlugValue(plug_transparency) + " );\n";

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
        std::cerr << "WARNING: Unsupported plug " << plug_name << " in Lambert shader " << dn.name().asChar() << std::endl;
        // throw exception ??? !!! FIXME!!!
    }

    return code_block;
}
