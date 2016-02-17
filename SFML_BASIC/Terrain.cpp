#include "stdafx.h"
#include "SFML/OpenGL.hpp"
#include "Terrain.h"
#include <cmath>
#include <iostream>

Terrain::Terrain(void)
{
	gridWidth = 99;
	gridDepth = 99;

	terrWidth=50; //size of terrain in world units
	terrDepth=50;
	vertices=NULL;
	texCoords = NULL;
	normals = NULL;
	
	//num squares in grid will be width*height, two triangles per square
	//3 verts per triangle
	 numVerts=gridDepth*gridWidth*2*3;


}

void Terrain::NormalVector(GLfloat p1[3], GLfloat p2[3], GLfloat p3[3], GLfloat n[3]){

	GLfloat v1[3], v2[3]; // two vectors

	//calculate two vectors lying on the surface
	// v1=p2-p1
	// v2=p3-p2

	for (int i = 0; i<3; i++){
		v1[i] = p2[i] - p1[i];
		v2[i] = p3[i] - p2[i];
	}

	// calculate cross product of two vectors ( n= v1 x v2)
	n[0] = v1[1] * v2[2] - v2[1] * v1[2];
	n[1] = v1[2] * v2[0] - v2[2] * v1[0];
	n[2] = v1[0] * v2[1] - v2[0] * v1[1];

} //done


Terrain::~Terrain(void)
{
	delete [] vertices;
	delete[] texCoords;
}

//interpolate between two values
float lerp(float start, float end, float t){
	return start+(end-start)*t;
}


void Terrain::setPoint(vector v, float x, float y, float z){
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

void Terrain::setPoint(vector2 v2, float u, float v){
	v2[0] = u;
	v2[1] = v;
}

//helper function to calculate height of terrain at a given point in space
//you will need to modify this significantly to pull height from a map
float  Terrain::getHeight(int x, int y){
	//get the height by getting the red component of colour at the x, y location and dividing by 255.f to get a float 0-1
	//multiply by const scaler otherwise its too small
	float height = ((heightPixels[y + x*heightMapWidth]).r / 255.f) * HEIGHT_SCALE;
	return height;
}

void Terrain::Init(sf::Image heightMap){
	
	delete [] vertices;//just in case we've called init before
	vertices=new vector[numVerts];
	delete[] texCoords;
	texCoords = new vector2[numVerts];
	delete[] normals;
	normals = new vector[numVerts];

	//get colour of pixels from height map
	heightMapWidth = heightMap.getSize().x;
	int heightMapHeight = heightMap.getSize().y;
	heightPixels = new sf::Color[heightMapWidth * heightMapHeight]; //1D array of sf::Colour which is the length of how many pixels are in the height map
	for (int y = 0; y < heightMapHeight; y++){ //loop through rows 
		for (int x = 0; x < heightMapWidth; x++){ //loop through columns
			int pixelNum = (x + y*heightMapWidth); //get the number of the pixel we are on
			heightPixels[pixelNum] = heightMap.getPixel(x, y); //get the colour of the pixel at this position
		}
	}

	GLfloat * a, *b, *c;
	GLfloat normal[3];

	//interpolate along the edges to generate interior points
	for(int i=0;i<gridWidth;i++){ //iterate left to right
		for(int j=0;j<gridDepth;j++){//iterate front to back
			int sqNum=(j+i*gridDepth); //get the current square
			int vertexNum=sqNum*3*2; //6 vertices per square (2 tris)

			//get bounds of current square. terrain is drawn around the origin
			float front=lerp(-terrDepth/2,terrDepth/2,(float)j/gridDepth); 
			float back =lerp(-terrDepth/2,terrDepth/2,(float)(j+1)/gridDepth);
			float left=lerp(-terrWidth/2,terrWidth/2,(float)i/gridWidth);
			float right=lerp(-terrWidth/2,terrWidth/2,(float)(i+1)/gridWidth);
			
			/*
			back   +-----+	looking from above, the grid is made up of regular squares
			       |tri1/|	'left & 'right' are the x cooded of the edges of the square
				   |   / |	'back' & 'front' are the y coords of the square
				   |  /  |	each square is made of two trianlges (1 & 2)
				   | /   |	
				   |/tri2|
			front  +-----+
			     left   right
				 */


			int repeatAmount = 3;
			//tri1
			a = vertices[vertexNum];
			setPoint(texCoords[vertexNum], ((float)i / gridWidth) * repeatAmount, ((float)j / gridDepth) * repeatAmount);
			setPoint(vertices[vertexNum++],left,getHeight(i,j),front);
			
			b = vertices[vertexNum];
			setPoint(texCoords[vertexNum], ((float)(i + 1) / gridWidth) * repeatAmount, ((float)j / gridDepth) * repeatAmount);
			setPoint(vertices[vertexNum++], right, getHeight((i + 1), j), front);

			c = vertices[vertexNum];
			setPoint(texCoords[vertexNum], ((float)(i + 1) / gridWidth) * repeatAmount, ((float)(j + 1) / gridDepth) * repeatAmount);
			setPoint(vertices[vertexNum++], right, getHeight((i + 1), (j + 1)), back);

			NormalVector(b, a, c, normal);
			AddNormal(vertexNum, normal);

			//tri2
			a = vertices[vertexNum];
			setPoint(texCoords[vertexNum], ((float)(i + 1) / gridWidth) * repeatAmount, ((float)(j + 1) / gridDepth) * repeatAmount);
			setPoint(vertices[vertexNum++], right, getHeight((i + 1), (j + 1)), back);

			b = vertices[vertexNum];
			setPoint(texCoords[vertexNum], ((float)i / gridWidth) * repeatAmount, ((float)(j + 1) / gridDepth) * repeatAmount);
			setPoint(vertices[vertexNum++], left, getHeight(i, (j + 1)), back);

			c = vertices[vertexNum];
			setPoint(texCoords[vertexNum], ((float)i / gridWidth) * repeatAmount, ((float)j / gridDepth) * repeatAmount);
			setPoint(vertices[vertexNum++], left, getHeight(i, j), front);

			NormalVector(b, a, c, normal);
			AddNormal(vertexNum, normal);
		}
	}
}

void Terrain::AddNormal(int lastIndex, GLfloat n[3])
{
	for (int i = 0; i < 3; i++)
	{
		setPoint(normals[lastIndex - i], n[0], n[1], n[2]);
	}
}


void Terrain::Draw(){
	glBegin(GL_TRIANGLES);
	for(int i =0;i<numVerts;i++){
		glColor3d(1.0, 1.0, 1.0); //color all white
		glTexCoord2fv(texCoords[i]); 
		glVertex3fv(vertices[i]);
		glNormal3fv(normals[i]);
	}
	glEnd();
}

float Terrain::getHeightScale(){
	return HEIGHT_SCALE;
}