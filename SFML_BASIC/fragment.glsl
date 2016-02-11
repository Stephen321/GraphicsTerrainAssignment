//textures
uniform sampler2D seaTexture;
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;

//interpolated height of the fragment
varying float height;

void main()
{
	vec4 colour; //the final colour
	//the possible colours
	vec4 seaColour = texture2D(seaTexture, fract(gl_TexCoord[0].st)); //loop up the colour of the texel at the interpolated tex coord
	vec4 grassColour = texture2D(grassTexture, fract(gl_TexCoord[0].st));
	vec4 rockColour = texture2D(rockTexture, fract(gl_TexCoord[0].st));
	
	//change colour based on heught
	if (height >= 0.70){
		colour = rockColour;
	}
	else if (height >= 0.5){ //blend rock and grass
		colour = mix(rockColour, grassColour, (0.70 - height) / 0.2); //blend colours depending on height
	}
	else if (height >= 0.1){
		colour = grassColour;
	}
	else if (height >= 0.05){ //blend grass and sea
		colour = mix(grassColour, seaColour, (0.1 - height) / 0.05);
	}
	else{
		colour = seaColour ;
	}

	//finally set the colour
	gl_FragColor = colour * gl_Color;
}