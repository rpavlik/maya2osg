#version 120

// Uniforms set from Maya2OSG exporter
uniform bool LocalViewer;

uniform int NumEnabledLights;
//int NumEnabledLights=1;

uniform sampler2D ColorTexture;
uniform sampler2D TranspTexture;

#define COLOR_TEXTURE 0
//#define TRANSP_TEXTURE 0


// From vertex shader
varying vec3 ecPosition3;
varying vec3 normal;

void DirectionalLight(in int i,
                      in vec3 normal,
                      inout vec4 ambient,
                      inout vec4 diffuse)
{
     float nDotVP;         // normal . light direction
     float pf;             // power factor

     nDotVP = max(0.0, dot(normal,
                   normalize(vec3(gl_LightSource[i].position))));

     ambient  += gl_LightSource[i].ambient;
     diffuse  += gl_LightSource[i].diffuse * nDotVP;
}

void PointLight(in int i,
                in vec3 eye,
                in vec3 ecPosition3,
                in vec3 normal,
                inout vec4 ambient,
                inout vec4 diffuse)
{
    float nDotVP;         // normal . light direction
    float attenuation;    // computed attenuation factor
    float d;              // distance from surface to light source
    vec3  VP;             // direction from surface to light position

    // Compute vector from surface to light position
    VP = vec3(gl_LightSource[i].position) - ecPosition3;

    // Compute distance between surface and light position
    d = length(VP);

    // Normalize the vector from surface to light position
    VP = normalize(VP);

    // Compute attenuation
    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +
                         gl_LightSource[i].linearAttenuation * d +
                         gl_LightSource[i].quadraticAttenuation * d * d);

    nDotVP = max(0.0, dot(normal, VP));

    ambient += gl_LightSource[i].ambient * attenuation;
    diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;
}					  

void SpotLight(in int i,
               in vec3 eye,
               in vec3 ecPosition3,
               in vec3 normal,
               inout vec4 ambient,
               inout vec4 diffuse)
{
    float nDotVP;           // normal . light direction
    float spotDot;          // cosine of angle between spotlight
    float spotAttenuation;  // spotlight attenuation factor
    float attenuation;      // computed attenuation factor
    float d;                // distance from surface to light source
    vec3 VP;                // direction from surface to light position

    // Compute vector from surface to light position
    VP = vec3(gl_LightSource[i].position) - ecPosition3;

    // Compute distance between surface and light position
    d = length(VP);

    // Normalize the vector from surface to light position
    VP = normalize(VP);

    // Compute attenuation
    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +
                         gl_LightSource[i].linearAttenuation * d +
                         gl_LightSource[i].quadraticAttenuation * d * d);

    // See if point on surface is inside cone of illumination
    spotDot = dot(-VP, normalize(gl_LightSource[i].spotDirection));

    if (spotDot < gl_LightSource[i].spotCosCutoff)
        spotAttenuation = 0.0; // light adds no contribution
    else
        spotAttenuation = pow(spotDot, gl_LightSource[i].spotExponent);

    // Combine the spotlight and distance attenuation.
    attenuation *= spotAttenuation;

    nDotVP = max(0.0, dot(normal, VP));

    ambient  += gl_LightSource[i].ambient * attenuation;
    diffuse  += gl_LightSource[i].diffuse * nDotVP * attenuation;
}


void main() {

	vec3 eye;
	if (LocalViewer)
	    eye = -normalize(ecPosition3);
	else
	    eye = vec3(0.0, 0.0, 1.0);

	// Clear the light intensity accumulators
	vec4 amb  = vec4(0.0);
	vec4 diff = vec4(0.0);

	// TO-DO : **** FIXME!!!
	// Adjust normal by bump map
	// ...

	// Loop through enabled lights, compute contribution from each
	int i;
	for (i = 0; i < NumEnabledLights; i++)
//	for (i = 0; i < 1; i++)
	{
	    if (gl_LightSource[i].position.w == 0.0)
	        DirectionalLight(i, normalize(normal), amb, diff);
	    else if (gl_LightSource[i].spotCutoff == 180.0)
	        PointLight(i, eye, ecPosition3, normalize(normal), amb, diff);
	    else
	        SpotLight(i, eye, ecPosition3, normalize(normal), amb, diff);
	}

	// NOTE: gl_FrontLightModelProduct.sceneColor = gl_FrontMaterial.emission + gl_FrontMaterial.ambient * gl_LightModel.ambient
	vec4 color = gl_FrontLightModelProduct.sceneColor +
	            amb * gl_FrontMaterial.ambient +
#ifdef COLOR_TEXTURE
	            diff * texture2D(ColorTexture, gl_TexCoord[COLOR_TEXTURE].st);
#else
	            diff * gl_FrontMaterial.diffuse;
#endif

#ifdef TRANSP_TEXTURE
	color.a = texture2D(TranspTexture, gl_TexCoord[TRANSP_TEXTURE]).a;
#endif

	gl_FragColor = color;
}
