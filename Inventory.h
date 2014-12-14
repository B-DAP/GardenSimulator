#ifndef Inventory_H
#define Inventory_H

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <math.h>    //allows use of sin and cos
#include <string>		//allows use of strings
#include <vector>		//allows use of vectors


using namespace::std;

//Contains the inventory which will indicate 
class Inventory{
	private:
		vector<vector<int> > items; //Stores the quantity of items in inventory		
	
	public:
		Inventory(); //constructor
		Inventory(vector<vector<int> >); //constructor with preset inventory
		void resetInventory(); //empties all items in inventory
		void upgradeInventory(); //maximizes the quantity of items stored
		void removeFromInventory(vector<int>, quantity); //removes quantity of specified item
		String getMenuItem(vector<int>); //forms string: 'itemName xitemQuantity'
		vector<int> getItemIndex(String); //returns corresponding index of item
		String getItemName(vector<int>); //returns corresponding name of item
		vector<vector<String> > getAllMenuItems(); //forms array of all strings: 'itemName xitemQuantity'
		
		
		
		float getXPos(); //returns x coordinate
		float getYPos(); //returns y coordinate
		float getXSpeed(); //returns x-direction speed
		float getYSpeed(); //returns y-direction speed
		std::vector<float> getColour(); //returns point colour
		void setXPos(float); //sets x coordinate
		void setYPos(float); //sets y coordinate
		void setXSpeed(float); //sets x-direction speed
		void setYSpeed(float); //sets y-direction speed
		void setColour(float, float, float); //sets point colour
};

#endif