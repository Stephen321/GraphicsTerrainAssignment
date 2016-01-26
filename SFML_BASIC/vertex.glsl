varying float height; //height of the current vertex

uniform float heightScale; //constant 

void main()
{
	height = gl_Vertex.y / heightScale; //divided by heightscale to get height from 0 -> 1
	
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0; //set tex coord (deprecated in newer versions 1.30+)
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_FrontColor = gl_Color; //pass the colour along to the next pahse (fragment shader)
}