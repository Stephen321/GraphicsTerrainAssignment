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
	typedef  GLfloat vector2[2];
	//array of vertices for the grid(this will be a triangle list)
	//I know, very inefficient, but let's run before we walk
	vector *vertices;
	vector2 *texCoords;
	sf::Color * heightPixels;
	int heightMapWidth;
	const float heightScale = 16.f;
	float getHeight(int x, int y);
	void setPoint(vector, float, float, float);
	void setPoint(vector2, float, float);
	
public:
	Terrain(void);
	~Terrain(void);

	void Init(sf::Image);
	void Draw();
	float getHeightScale();
};

