//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 
 
#include "SFML/Graphics.hpp" 
#include "SFML/Graphics/Shader.hpp"
#include "SFML/OpenGL.hpp" 
#include <iostream> 
  
 
#include "Terrain.h"
#include "Camera.h"




int main() 
{ 
    // Create the main window 

	

	bool wireFrame = false;
    int width=600,height=600;
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 16;  // Request 2 levels of antialiasing
	sf::RenderWindow App(sf::VideoMode(width, height, 32), "SFML OpenGL", sf::Style::Close, Settings);
    // Create a clock for measuring time elapsed     
    sf::Clock Clock; 

	aiVector3D position(0,10,-30);
	Camera camera;
    camera.Init(position); //create a camera
      
    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.3f, 0.3f, 0.6f, 0.f); //background colour
	glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); 

	//to enable texture tiling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   
    //// Setup a perspective projection & Camera position 
    glMatrixMode(GL_PROJECTION); 

	glEnable(GL_LIGHTING); // switch on lighting
	glEnable(GL_LIGHT0); // switch on light0

	GLfloat light_color[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light_position[] = { 15.0f, 15.0f, 0.0f, 0.0f };


	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color); // set color of diffuse component
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color); // set color of specular component

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   // set position
	

	GLfloat materialAmbDiff[] = { 1, 1, 1, 1.0f }; // create an array of RGBA values


	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiff);
	// set the diffuse & ambient reflection colour for the front of faces

	GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // create an array of RGBA values (White)
	GLfloat materialShininess[] = { 40 }; // select value between 0-128, 128=shiniest


	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular); // set the colour of specular reflection
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess); // set shininess of the material
    glLoadIdentity(); 
     
    //set up a 3D Perspective View volume
    gluPerspective(90.f, (float)width/height, 1.f, 300.0f);//fov, aspect, zNear, zFar 
 

	//load in different textures
	sf::Texture seaTexture;
	seaTexture.loadFromFile("sea.png");
	sf::Texture grassTexture;
	grassTexture.loadFromFile("grass.png");
	sf::Texture rockTexture;
	rockTexture.loadFromFile("rock.png");

	//load the shader
	sf::Shader shader;
	//all the lighting & texture blending code should  be put in 'fragment.glsl'
	if(!shader.loadFromFile("vertex.glsl","fragment.glsl")){
        exit(1);
    }
	//set textures in the shader
	shader.setParameter("seaTexture", seaTexture);
	shader.setParameter("grassTexture", grassTexture);
	shader.setParameter("rockTexture", rockTexture);
	//load the height map image
	sf::Image heightMap;
	heightMap.loadFromFile("heightmap.png");

	//Create our Terrain
	Terrain terrain;
	terrain.Init(heightMap); //create from height map
	shader.setParameter("heightScale", terrain.getHeightScale()); //set the height scaler in the shader

    // Start game loop 
    while (App.isOpen()) 
    { 
        // Process events 
        sf::Event Event; 
        while (App.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                App.close(); 
   
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                App.close(); 
             
			//update the camera
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::I))
				wireFrame = !wireFrame;
			//update the camera
			camera.Update(Event);
        } 
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);   // set position
        //Prepare for drawing 
        // Clear color and depth buffer 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sf::Shader::bind(&shader); //shader has to be binded here to allow changing textures
		//glEnable(GL_TEXTURE_2D);

		if (wireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   
        // Apply some transformations 
        //initialise the worldview matrix
		glMatrixMode(GL_MODELVIEW); 
        glLoadIdentity(); 

		//get the viewing transform from the camera
		camera.ViewingTransform();


		//make the world spin
		//TODO:probably should remove this in final
		//static float ang=0.0;
		//ang+=0.01f;
		//glRotatef(ang*2,0,1,0);//spin about y-axis
		

		
		//draw the world
		terrain.Draw();

		   
        // Finally, display rendered frame on screen 
        App.display(); 
    } 
   
    return EXIT_SUCCESS; 
}
