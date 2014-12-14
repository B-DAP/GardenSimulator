#ifndef Plant_H
#define Plant_H

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>		//allows use of strings
#include <vector>		//allows use of vectors
#include <cstring>	//allows use of strcmp

#include "customLib.h"
#include "Weather.h"

using namespace::std;



//Contains the plant state and  object loader and properties. 
class Plant
{
public:
	enum Type{Soil, Flower, Tree, Grass};

private:
	float age;					//Age determines how large/what form the plant is
	float maxAge;				//Limit to how old a plant can get -- Limits the plants size
	Type pType;					//Type of plant -- determines what it looks like and how it acts
	float plantColour[3];	//Contains the RGB values for the Plant

public:
	//constructor
	Plant(Type type);
	
	void grow();
	void draw(Weather::Type forecast);
	Type getType();
};

#endif
