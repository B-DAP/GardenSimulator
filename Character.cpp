#include "Character.h"

//Pokemon constructor
Character::Character(float* inPos)
{
	//Gives the Character an inital position
	pos[0] = inPos[0];
	pos[1] = inPos[1] + 0.55;
	pos[2] = inPos[2];
	
	charColour[0] = .5; 
	charColour[1] = .5;
	charColour[2] = 0;

	//load the character's point structure
	loadCharacter();
}

Character::~Character()
{
	vertices_List.clear();
	normals_List.clear();
}

void Character::move(float* newPos)
{
	//update the Characters position
	pos[0] = newPos[0];
	pos[2] = newPos[2];
}

void Character::draw(float cAngle)
{
	float scale = 0.1;
		
	glColor3f(charColour[0],charColour[1],charColour[2]);
	
	glPushMatrix();{
		//glTranslatef(pos[0],pos[1],pos[2]);
		glScalef(1.5*scale, 1.5*scale, 1.5*scale);


/*

			case GLUT_KEY_LEFT :
				aAngle -= 0.05f;
				aLookX = sin(aAngle);
				aLookZ = -cos(aAngle);
				break;
			case GLUT_KEY_RIGHT :
				aAngle += 0.05f;
				aLookX = sin(aAngle);
				aLookZ = -cos(aAngle);
				break;
*/




		//Rotate the Character so they are facing the correct direction
		//glRotatef(180*-cos(-cAngle) + 180*sin(-cAngle) ,0,1,0);

		//Go through the verices list drawing every triangle of the Character
		for(int idx = 0; idx < vertices_List.size()/3; idx++)
		{
			glBegin(GL_POLYGON);
			for(int i = 0; i < 3; i++)//3 points together are a triangle face for the pokemon model
			{	
				glNormal3f(normals_List[(idx*3)+i].x,normals_List[(idx)*3 +i].y,normals_List[(idx*3)+i].z);//Normal used for lighting
				glVertex3f(vertices_List[(idx*3)+i].x,vertices_List[(idx*3) + i].y, vertices_List[(idx*3) + i].z);
			}
			glEnd();
		}	
	}glPopMatrix();	
}

//******PLEASE NOTE********//
/* I obtained and modified this code from doing an object loader tutorial 
** from the website "www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/"	
*/
bool Character::loadCharacter()
{
	//object loader code
	const char * fp;
	fp = "./Objects/Pikachu.obj";

	vector< unsigned int> vertexIndices;
	vector< unsigned int> normalIndices;
	vector<vec3> temp_vertices;
	vector<vec3> temp_normals;
		
	//Open OBJ file
	FILE * file = fopen(fp, "r");
	if(file == NULL)
	{
		printf("ERROR OPENING FILE\n");
		return false;//return false -- indicate that the object loading was a failure
	}
	
	char lineHeader[128];

	while(fscanf(file, "%s", lineHeader) != EOF)
	{

		if(strcmp(lineHeader, "v") == 0)//Line is a vertex
		{
			vec3 vertex;
			fscanf(file,"%f %f %f\n",&vertex.x, &vertex.y, &vertex.z);			
			temp_vertices.push_back(vertex);
		}
		else if(strcmp(lineHeader,"vn") == 0)//Line is a normal
		{
			vec3 normal;
			fscanf(file, "%f %f %f\n",&normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if(strcmp(lineHeader,"f") == 0)//Line is a face
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			fscanf(file," %d/%d/%d %d/%d/%d %d/%d/%d\n",
						&vertexIndex[0], &uvIndex[0], &normalIndex[0],
						&vertexIndex[1], &uvIndex[1], &normalIndex[1],
						&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	
	//For each vertex of each triangle
	for(unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		//Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int normalIndex = normalIndices[i];

		//Get the attributes thanks to the index
		vec3 vertex = temp_vertices[ vertexIndex-1 ];
		vec3 normal = temp_normals[ normalIndex-1 ];
		
		//Put the attributes in buffers
		vertices_List.push_back(vertex);
		normals_List.push_back(normal);
	}

	return true;
}


//***** Getter Functions *****//
float* Character::getPos()
{
	return pos;
}
