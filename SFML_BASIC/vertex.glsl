varying float height; //height of the current vertex
varying vec4 specularLight;
uniform float heightScale; //constant 

void main()
{
	height = gl_Vertex.y / heightScale; //divided by heightscale to get height from 0 -> 1
	
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0; //set tex coord (deprecated in newer versions 1.30+)
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_FrontColor = gl_Color; //pass the colour along to the next pahse (fragment shader)

	vec3 normal, lightDir;
	vec4 diffuse, ambient, globalAmbient;
	float NdotL, NdotHV = 0.0f;
	vec4 specular=vec4(0,0,0,0);
	diffuse = gl_FrontMaterial.diffuse  * gl_LightSource[0].diffuse;
	normal = normalize(gl_NormalMatrix * gl_Normal);

	vec4 pointEyeSpace=(gl_ModelViewMatrix*gl_Vertex);

	lightDir = normalize(vec3(gl_LightSource[0].position)-pointEyeSpace.xyz);

	NdotL = max(dot(normal, lightDir), 0.0);
	
	vec4 cameraDir=-pointEyeSpace;
	vec3 halfVector=normalize((lightDir + cameraDir)).xyz;
	
	if (NdotL > 0.0) {

		// normalize the half-vector, and then compute the
		// cosine (dot product) with the normal
		NdotHV = max(dot(normal, halfVector.xyz),0.0);
		specular = gl_FrontMaterial.specular * gl_LightSource[0].specular *
				pow(NdotHV,gl_FrontMaterial.shininess);
	}
	
	//specularLight = specular;
	gl_FrontColor = globalAmbient + NdotL * diffuse + ambient;
	
	gl_Position = ftransform();
}