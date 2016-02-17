#pragma once
#include "SFML\Graphics.hpp"
class Terrain
{
	//size of the mesh forming the terrain
	int gridWidth,gridDepth;
	int numVerts;
	//size of the terrain in world_coords
	float terrWidth,terrDepth;

	typedef  GLfloat vector[3];
	typedef  GLfloat vector2[2]; //make a new typedef for 2 long array of GLfloats 
	//array of vertices for the grid(this will be a triangle list)
	//I know, very inefficient, but let's run before we walk
	vector *vertices;
	vector2 *texCoords; 
	vector *normals;
	sf::Color * heightPixels;
	int heightMapWidth;
	const float HEIGHT_SCALE = 16.f;
	float getHeight(int x, int y);

	void setPoint(vector, float, float, float);
	void setPoint(vector2, float, float); //set 2 points 
	void NormalVector(GLfloat p1[3], GLfloat p2[3], GLfloat p3[3], GLfloat n[3]);
	void AddNormal(int lastIndex, GLfloat n[3]);
	
public:
	Terrain(void);
	
	~Terrain(void);

	void Init(sf::Image);
	void Draw();
	float getHeightScale();
};

