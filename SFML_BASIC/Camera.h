#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GL/glu.h" 



class Camera
{
	static aiVector3D zero, xaxis, yaxis, zaxis;

public:
    aiVector3D position;
    aiVector3D forward;
    aiVector3D up;
	aiVector3D lookat;

    float forwardSpeed;
	float rotationSpeed;

     
	Camera() :forwardSpeed(0.5f), rotationSpeed(0.1f) {}
 
    void Init(aiVector3D& p=zero, aiVector3D& f=zaxis, aiVector3D& u=yaxis)
	{
        position=p;
        forward=f;
        up=u;
    }
 
	void Update(sf::Event e)
	{//respond to keyboard events
/////////////////////MOVEMENT/////////////////////////////////////////////////////////////////////////
			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::D))
                MoveLeftRight(-1);

			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::A))
                MoveLeftRight(+1);

 
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::W))
                MoveForwardBack(1);

            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::S))
                MoveForwardBack(-1);
            
/////////////////////VERTICAL MOVEMENT/////////////////////////////////////////////////////////////////////////
			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Q))
                MoveUpDown(1);
            
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::E))
                MoveUpDown(-1);
            
/////////////////////PROJECTIONS/////////////////////////////////////////////////////////////////////////
			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::O))
				OrthographicProjection();
			
			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::P))
				perspectiveProjection();
			
/////////////////////TURNING/////////////////////////////////////////////////////////////////////////
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Right))
                TurnRightLeft(-1);
            
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Left))
                TurnRightLeft(1);
            
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Up))
               TurnUpDown(-1);
            
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Down))
                TurnUpDown(1);
	}

    void MoveLeftRight(int dir)
	{ //Dir=+1=>Right, dir=-1=> Left
		position += xaxis*(forwardSpeed*dir);
    }
	void MoveUpDown(int dir)
	{ //Dir=+1=>Right, dir=-1=> Left
		position += up*(forwardSpeed*dir);
    }
    void MoveForwardBack(int dir)
	{ //Dir=+1=>Forward, dir=-1=> Back
		position += forward*(forwardSpeed*dir);
    }

	void OrthographicProjection()
	{ //Dir=+1=>Forward, dir=-1=> Back
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-20, 20, -20, 20,100,-200);
		glMatrixMode(GL_MODELVIEW);
	}

	void perspectiveProjection()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.f, (float)800 / 600, 1.f, 300.0f);//fov, aspect, zNear, zFar
	}
 
    void TurnRightLeft(int dir)
	{ //Dir=+1=>Right, dir=-1=> Left
		forward.z = forward.z*cos(rotationSpeed*dir) - forward.x*sin(rotationSpeed*dir);
		forward.x = forward.z*sin(rotationSpeed*dir) + forward.x*cos(rotationSpeed*dir);
    }
         
    void TurnUpDown(int dir)
	{ //Dir=+1=>Up, dir=-1=> Down
		forward.y -= tan(rotationSpeed * dir);
    }


 
    void ViewingTransform()
	{
        gluLookAt(	position.x,position.y,position.z,// camera position
				  forward.x + position.x , forward.y + position.y, forward.z + position.z, //look at this point //TODO: BUG here!! what is it ??
					0,1,0); //camera up
    }
 
};

//create some default vectors
aiVector3D Camera::zero(0.0f);
aiVector3D Camera::xaxis(1.0f, 0.0f, 0.0f);
aiVector3D Camera::yaxis(0.0f,1.0f,0.0f);
aiVector3D Camera::zaxis(0.0f,0.0f,1.0f);
