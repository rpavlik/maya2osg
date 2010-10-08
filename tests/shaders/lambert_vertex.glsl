// Uniforms from Maya shader
// (most of them are exported to OpenGL Material and accessed through built-in uniforms)

// Texture units used
uniform int numTexCoordSets;

varying vec3 ecPosition3;
varying vec3 normal;

void main() {
	// Transform vertex from object space to clip space
	gl_Position = ftransform();

	// NOTE: Lighting computations are performed in eye coordinates
	// because OpenGL specifies that light positions are transformed
	// by the modelview matrix when they are provided to OpenGL

	// Vertex position in eye coordinates (and projected)
	vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
	ecPosition3 = (vec3(ecPosition)) / ecPosition.w;

	// Normals (in eye coordinates, we will normalize them after interpolation)
	normal = gl_NormalMatrix * gl_Normal;

	// For every used texture unit... *** TO-DO - FIXME!!!!
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}
