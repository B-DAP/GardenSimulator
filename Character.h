#ifndef Character_H
#define Character_H

#include <GL/glut.h>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>    //allows use of sin and cos
#include <string>		//allows use of strings
#include <vector>		//allows use of vectors
#include <cstring>	//allows use of strcmp


#include "customLib.h"

using namespace::std;


//Contains the Particle list as well as attributes for the character that will be 
//able to move around the screen. also contains the object loader
class Character
{
public:

private:
	float pos[3];				//position of character
	float charColour[3];			//Contains the RGB values for the character
	
	//Object Details
	bool loadCharacter();//object loader function

	vector<vec3> vertices_List;
	vector<vec3> normals_List;
public:
	//constructor
	Character(float* pos);
	//deconstructor
	~Character();

	void move(float* newPos, float mapSize);
	void draw(float cAngle);
	
	float* getPos();	
};

#endif
