uniform sampler2D seaTexture;
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;

varying float height;

void main()
{
	vec4 colour;
	vec4 seaColour = texture2D(seaTexture, gl_TexCoord[0].st);
	vec4 grassColour = texture2D(grassTexture, gl_TexCoord[0].st);
	vec4 rockColour = texture2D(rockTexture, gl_TexCoord[0].st);
	if (height >= 0.75){
		colour = rockColour;
	}
	else if (height >= 0.6){ //blend rock and grass
		colour = mix(rockColour, grassColour, (0.75 - height) / 0.25);
	}
	else if (height >= 0.1){
		colour = grassColour;
	}
	else if (height >= 0.05){ //blend grass and sea
		colour = mix(grassColour, seaColour, (0.1 - height) / 0.05);
	}
	else{
		colour = seaColour;
	}
	gl_FragColor = colour;
}