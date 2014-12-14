#include "Plant.h"

//Plant constructor
Plant::Plant(Type type)
{
	//Assigns the Plant a type
	pType = type;

	//all Plants start out with an age of 0
	age = 0;

	switch(type)
	{	
		case Flower://flowers are yellow -- for now
			maxAge = 1500;	
			plantColour[0] = 0.75;
			plantColour[1] = 0.75;
			plantColour[2] = 0;
			break;
	
		case Tree://trees shall be green -- for now
			maxAge = 2500;
			plantColour[0] = 0;
			plantColour[1] = 0.8;
			plantColour[2] = 0.1;
			break;

		default://For non-plant object who don't use the maxAge or planColour variables
			maxAge = 1;
			plantColour[0] = 0;//set the variables to values so so we don't potentially access null variables
			plantColour[1] = 0;
			plantColour[2] = 0;
			break;
	}
}
void Plant::grow()//growing increases the plants age, which increases it's size
{
	if(age < maxAge)
	{
		age++;
	}
}
void Plant::draw(Weather::Type forecast)//draw the plant
{
	//draw grass if the ground has not been tilled
	if(pType == Grass)
	{
		if(forecast == Weather::Snow){glColor3f(0.1,0.15,0.1);}//draw snow on the ground
		else{glColor3f(0,0.1,0);}
	}	
	else//draw the dirt
	{
		if(forecast == Weather::Snow){glColor3f(.15,.1,0.05);}//draw snow on the ground
		else{glColor3f(0.1,0.05,0);}
	}
	
	glPushMatrix();{
		glScalef(2,0.1,2);
		glutSolidCube(1);
	}glPopMatrix();
	
	//draw the Plant
	glColor3f(plantColour[0],plantColour[1],plantColour[2]);//set the objects colour
	glPushMatrix();{
		glScalef(age/maxAge,age/maxAge,age/maxAge);//scale the object by size
		if(pType == Flower)//Flowers		
			glutSolidSphere(0.5,16,16);//draw the object
		if(pType == Tree)//Trees
		{
			glScalef(1,5,1);
			glutSolidCube(1);
		}
	}glPopMatrix();
}

Plant::Type Plant::getType()
{
	return pType;
}
