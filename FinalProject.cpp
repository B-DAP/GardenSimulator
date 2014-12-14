//Graphics Libraries
#include <GL/glut.h>

//C++ Libraries
#include <list>
#include <stdio.h>
#include <math.h>  //allows use of the pow function
#include <vector>  //allows use of vectors

//Custom Build Classes
#include "Plant.h"
#include "Character.h"
#include "Weather.h"

using namespace std;

#define PI 3.14159265

//Map Dimensions
const int size = 50;//How large is our garden going to be
float floorDim[] = {size,1,size};

//Adventure Mode -- 3rd Person Variables
float pAngle = 0.0f;

//Lighting Variables
GLfloat light_pos0[] = {-size/2, size/2, 0, 1};

//Objects
	//Plants
const int plantListSize = size/2;//const that determines how many plant "blocks we have"
Plant* plantList[plantListSize][plantListSize];	//list of all Plants
	//Character
float charPos[3] = {0, 0, 0};
Character* player = new Character(charPos);
	//Weather
Weather::Type forecast = Weather::Clear;//Determines what weather effects are happening today
list<Weather*> weatherList;	//list of all active precipitation particles
int weatherCount = 0; //counter that is used to create Particles every 4th frame

void precipitate()
{
	float aPos[3];//Position where new particles shall apear from
	float aDir[3];//Particles are given a initial direction
	float randNum;
	//Create 1 to 25 Particles on average
	do{
		aPos[0] = (rand()%size) - size/2;
		aPos[1] = 15;
		aPos[2] = (rand()%size) - size/2;

		if(forecast == Weather::Snow)
		{
			randNum = (rand()%800) - 400;//Snow floats down randomly
			aDir[0] = randNum/20000;
			randNum = (rand()%800) - 400;
			aDir[2] = randNum/20000;			
		}
		else
		{	
			aDir[0] = 0;
			aDir[2] = 0;
		}
		aDir[1] = 0;
	
		//Add the new patricle to the list
		weatherList.push_back(new Weather(aPos, aDir, forecast));
	}while(rand()%25 != 0);
}

//Predicate used to determine if projectile is 8 seconds or older
bool OldParticle(Weather* x){return x->getAge() > 480;};

void drawWeather()
{	
	//go through the list drawing then moving every particle
	for(list<Weather*>::const_iterator iter = weatherList.begin(); iter != weatherList.end(); iter++)
	{
		(*iter)->draw();
		(*iter)->move(size);
	}

	//after drawing all the particles, remove particles that have been alive for more than 8 seconds or have already hit the ground
	weatherList.remove_if(OldParticle);
}

void special(int key, int x, int y)
{
float fraction = 1.0f;
		/* arrow key presses move the camera */
		float *tempPos = player->getPos();
		float newPos[3];
		newPos[0] = tempPos[0];
		newPos[2] = tempPos[2];
	switch (key) 
		{
			case GLUT_KEY_LEFT :
					pAngle -= 2.0f;
				break;
			case GLUT_KEY_RIGHT :
					pAngle += 2.0f;
				break;

			case GLUT_KEY_UP ://if moving won't put us off the board then move forward
				newPos[0] += 0.5*cos(pAngle*PI/180);			
				newPos[2] += 0.5*sin(pAngle*PI/180);	
				player->move(newPos,size);
				break;
			case GLUT_KEY_DOWN ://if moving won't put us off the board then move backwards
				newPos[0] -= 0.5*cos(pAngle*PI/180);				
				newPos[2] -= 0.5*sin(pAngle*PI/180);
				player->move(newPos,size);				
				break;
		}
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'q':
		case 'Q':
		case 27:
			exit (0);
			break;

		//Movement Keys
		// --- User is able to use WASD keys to move as well
		// --- User is able to move where the camera looks at in map mode
		case 'w':
		case 'W':
			special(GLUT_KEY_UP,0,0);
			break;

		case 's':
		case 'S':
			special(GLUT_KEY_DOWN,0,0);		
			break;

		case 'a':
		case 'A':
			special(GLUT_KEY_LEFT,0,0);			
			break;
	
		case 'd':
		case 'D':
			special(GLUT_KEY_RIGHT,0,0);
			break;

		case 'p':
			forecast = Weather::Snow;
			break;
		case 'l':
			forecast = Weather::Rain;
			break;

		case ' ':
			float *playerPos = player->getPos();
			int plantBlockX = (playerPos[0] + (size/2))/2;
			int plantBlockZ = (playerPos[2] + (size/2))/2;
			Plant::Type plantType = plantList[plantBlockX][plantBlockZ]->getType();
								
			switch(plantType)
			{
				case Plant::Grass ://Tile the ground
					delete plantList[plantBlockX][plantBlockZ];//delete old object to prevent memory leaks	
					plantList[plantBlockX][plantBlockZ] = new Plant(Plant::Soil);
					break;			

				case Plant::Soil ://Plant a flower if the soil is ready
					delete plantList[plantBlockX][plantBlockZ];//delete old object to prevent memory leaks	
					plantList[plantBlockX][plantBlockZ] = new Plant(Plant::Flower);
					break;
		
				case Plant::Flower ://Pick the Flower
					delete plantList[plantBlockX][plantBlockZ];//delete old object to prevent memory leaks	
					plantList[plantBlockX][plantBlockZ] = new Plant(Plant::Soil);		
					break;							
			}			
			break;	
	}
}

void init(void)
{	
	//initalizes all plant blocks
	for(int i = 0; i < plantListSize; i++)
	{
		for(int j = 0; j < plantListSize; j++)
		{
			plantList[i][j] = new Plant(Plant::Grass);
		}
	}


	//set materials
	float m_amb[] = {0.9, 0.9, 0.9, 1.0};
	float m_dif[] = {0.1, 0.1, 0.1, 1.0};
	float m_spec[] = {0.1, 0.1, 0.1, 1.0};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);

	glEnable(GL_COLOR_MATERIAL);
	
	//Light Properties
	glEnable(GL_LIGHTING);

	//set light properties for sun
	float amb0[4] = {.2, .2, .2, 1};
	float diff0[4] = {.6, .6, .6, 1}; 
	float spec0[4] = {.4, .4, .4, 1};
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
		
	glEnable(GL_LIGHT0);


   glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0);
	glEnable(GL_CULL_FACE);//BackFace Culling enabled
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 600);
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	float *playerSpot = player->getPos();

	gluLookAt(playerSpot[0] -10*cos(pAngle*PI/180),5,playerSpot[2] -10*sin(pAngle*PI/180), playerSpot[0],1.9,playerSpot[2], 0.0f,1.0f,0.0f);

	//Draw the Character
	glPushMatrix();{
		player->draw(pAngle);
	}glPopMatrix();

/***** -- Lighting -- *****/
	//As time goes by sun moves across the sky
	light_pos0[0] += 0.3;
	if(light_pos0[0] > size)//new day
	{
		light_pos0[0] = -size;//sun go's back to start position to signify a new day
	}

	//Draw the Sun(light0)
	glPushMatrix();
		glTranslatef(light_pos0[0],light_pos0[1],light_pos0[2]);
		glColor3f(1,1,0);
		glutSolidSphere(3,16,16);
	glPopMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);
/*************************/

/***** -- The Scene -- *****/
glPushMatrix();
	
	//Draw the Weather Effects
	drawWeather();

	//Draw the Plant Objects
	for(int i = 0; i < plantListSize; i++){
		for(int j = 0; j < plantListSize; j++)
		{	
			glPushMatrix();{	
				glTranslatef((-size/2) + i*2 +1, 0.49, (-size/2) + j*2 + 1);
				plantList[i][j]->draw(forecast);//draw the plant				
				plantList[i][j]->grow();//grow all the plants
			}glPopMatrix();
		}
	}
glPopMatrix();
/***************************/
	glutSwapBuffers();
}

void timer(int value)
{
	weatherCount++;
	if(weatherCount > 4)//Draw more weather particles on every 4th frame
	{
		weatherCount = 0;
		if(forecast != Weather::Clear)//Only create particles if it is no clear
		{
			precipitate();
		}
	}

	glutTimerFunc(16,timer,0);//60 fps
	glutPostRedisplay();
}


/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Garden Simulator");	//creates the window

	glutDisplayFunc(display);	


	//Event Functions
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutTimerFunc(16,timer,0);

	glEnable(GL_DEPTH_TEST);
	init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}
