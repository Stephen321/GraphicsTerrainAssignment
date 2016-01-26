varying float height;

uniform float heightScale;

void main()
{
	height = gl_Vertex.y / heightScale;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0; //not used - https://www.opengl.org/wiki/GLSL_:_common_mistakes

	gl_FrontColor = gl_Color; //pass the colour along to the next pahse (fragment shader)
}