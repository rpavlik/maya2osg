// Uniforms set from Maya2OSG exporter
bool LocalViewer = true;
int NumEnabledLights = 1;
bool SeparateSpecular = false;

// From vertex shader
varying vec3 ecPosition3;
varying vec3 normal;

void DirectionalLight(in int i,
                      in vec3 normal,
                      inout vec4 ambient,
                      inout vec4 diffuse,
                      inout vec4 specular)
{
     float nDotVP;         // normal . light direction
     float nDotHV;         // normal . light half vector
     float pf;             // power factor

     nDotVP = max(0.0, dot(normal,
                   normalize(vec3(gl_LightSource[i].position))));
     nDotHV = max(0.0, dot(normal, vec3(gl_LightSource[i].halfVector)));

     if (nDotVP == 0.0)
         pf = 0.0;
     else
         pf = pow(nDotHV, gl_FrontMaterial.shininess);

     ambient  += gl_LightSource[i].ambient;
     diffuse  += gl_LightSource[i].diffuse * nDotVP;
     specular += gl_LightSource[i].specular * pf;
}

void PointLight(in int i,
                in vec3 eye,
                in vec3 ecPosition3,
                in vec3 normal,
                inout vec4 ambient,
                inout vec4 diffuse,
                inout vec4 specular)
{
    float nDotVP;         // normal . light direction
    float nDotHV;         // normal . light half vector
    float pf;             // power factor
    float attenuation;    // computed attenuation factor
    float d;              // distance from surface to light source
    vec3  VP;             // direction from surface to light position
    vec3  halfVector;     // direction of maximum highlights

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

    halfVector = normalize(VP + eye);

    nDotVP = max(0.0, dot(normal, VP));
    nDotHV = max(0.0, dot(normal, halfVector));

    if (nDotVP == 0.0)
        pf = 0.0;
    else
        pf = pow(nDotHV, gl_FrontMaterial.shininess);

    ambient += gl_LightSource[i].ambient * attenuation;
    diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;
    specular += gl_LightSource[i].specular * pf * attenuation;
}					  

void SpotLight(in int i,
               in vec3 eye,
               in vec3 ecPosition3,
               in vec3 normal,
               inout vec4 ambient,
               inout vec4 diffuse,
               inout vec4 specular)
{
    float nDotVP;           // normal . light direction
    float nDotHV;           // normal . light half vector
    float pf;               // power factor
    float spotDot;          // cosine of angle between spotlight
    float spotAttenuation;  // spotlight attenuation factor
    float attenuation;      // computed attenuation factor
    float d;                // distance from surface to light source
    vec3 VP;                // direction from surface to light position
    vec3 halfVector;        // direction of maximum highlights

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

    halfVector = normalize(VP + eye);

    nDotVP = max(0.0, dot(normal, VP));
    nDotHV = max(0.0, dot(normal, halfVector));

    if (nDotVP == 0.0)
        pf = 0.0;
    else
        pf = pow(nDotHV, gl_FrontMaterial.shininess);

    ambient  += gl_LightSource[i].ambient * attenuation;
    diffuse  += gl_LightSource[i].diffuse * nDotVP * attenuation;
    specular += gl_LightSource[i].specular * pf * attenuation;
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
	vec4 spec = vec4(0.0);
	
	// Loop through enabled lights, compute contribution from each
	int i;
	for (i = 0; i < NumEnabledLights; i++)
	{
	    if (gl_LightSource[i].position.w == 0.0)
	        DirectionalLight(i, normalize(normal), amb, diff, spec);
	    else if (gl_LightSource[i].spotCutoff == 180.0)
	        PointLight(i, eye, ecPosition3, normalize(normal), amb, diff, spec);
	    else
	        SpotLight(i, eye, ecPosition3, normalize(normal), amb, diff, spec);
	}

	vec4 _color = gl_FrontLightModelProduct.sceneColor +
	            amb * gl_FrontMaterial.ambient +
	            diff * gl_FrontMaterial.diffuse;

//	if (SeparateSpecular)
//	    gl_FrontSecondaryColor = spec * gl_FrontMaterial.specular;
//	else
	    _color += spec * gl_FrontMaterial.specular;

	gl_FragColor = _color;
}
