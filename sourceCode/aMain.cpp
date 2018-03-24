#include "..\include\freeglut.h"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "ship.h"
#include "lasers.h"


using namespace std;

//area of declarations and creating instances
GLdouble PI = 3.14159;	//Mathematical constant used for cos and sin functions
void spaceKeyRelease(unsigned char key, GLint x, GLint y);
void arrowKeyDown(int key, int x, int y);
void arrowKeyReleased(int key, int x, int y);
vector<GLfloat> red(4);
vector<GLfloat> green(4);
vector<GLfloat> blue(4);
ship *myShip;
GLfloat shipX = 0.0, shipY = 0.0, shipSize = 1;

GLfloat angle = 0.0;
bool laser = false;
GLfloat laserX = 0.0, laserY = 0.0, laserSize = 0.5;
bool buffer[3];

GLboolean upKeyPressed = false;
GLboolean downKeyPressed = false;
GLboolean leftKeyPressed = false;
GLboolean rightKeyPressed = false;
GLboolean spaceBarPressed = false;


int enemyCount = 0;
GLfloat enemyAngle = 0.0;
vector <lasers> laserManager;
vector <GLint> coordinateX(4);
vector <GLint>coordinateY(4);
vector <GLboolean>enemyActive(4);
vector <GLint>enemySize(4);
vector <GLint>enemyType(4);
GLfloat Colors[16];

GLint myScore = 0;


bool patternOne = false;
bool patternTwo = false;
bool noPattern = true;

GLubyte pattern_One[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
GLubyte pattern_Two[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x1f, 0x80, 0x1f, 0xc0, 0x0f, 0xc0, 0x3f, 0x80,
	0x03, 0xf5, 0xff, 0x00, 0x07, 0xfd, 0xff, 0xf8,
	0x1f, 0xfc, 0xff, 0x08, 0xff, 0x03, 0xbf, 0x70,
	0x03, 0x10, 0x40, 0x40, 0x08, 0x88, 0x8c, 0x20,
	0x05, 0x05, 0x04, 0x40, 0x02, 0x82, 0x14, 0x40,
	0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
};
void displayBackground()
{
	if (patternOne) {
		glEnable(GL_POLYGON_STIPPLE);
		glPolygonStipple(pattern_One);
		glRectf(-1920.0, -1080.0, 1920.0, 1080.0);
		glDisable(GL_POLYGON_STIPPLE);
	}
	else if (patternTwo)
	{
		glEnable(GL_POLYGON_STIPPLE);
		glPolygonStipple(pattern_Two);
		glRectf(-1920.0, -1080.0, 1920.0, 1080.0);
		glDisable(GL_POLYGON_STIPPLE);
	}
}
void createMenu(GLint id)
{
	if (id == 1)
	{
		patternOne = true;
		patternTwo = false;
		noPattern = false;
	}
	else if (id == 2)
	{
		patternOne = false;
		patternTwo = true;
		noPattern = false;
	}
	else if (id == 3)
	{
		patternOne = false;
		patternTwo = false;
		noPattern = true;
	}
	else
	{
		patternOne = false;
		patternTwo = false;
		noPattern = true;
	}
	glutPostRedisplay();
}
GLboolean detectCollisions(GLfloat radius, GLfloat eX, GLfloat eY, GLfloat lX, GLfloat lY)
{
	GLfloat distX = eX - lX;
	GLfloat distY = eY - lY;

	GLfloat distance = sqrt(distX * distX + distY * distY);
	if (distance <= (radius +0.2))
	{

		myScore += 10;
		enemyCount -= 1;
		return true;
	}
	else return false;
}
void drawSquare(GLfloat length, GLfloat x, GLfloat y);
GLint proximity();
void timer(int value);

void populate() 
{
	for (int i = 0; i < coordinateX.size(); i++)
	{
		GLint randForPosOrNeg = rand() % 2+1;
		GLint randomCoordinate = proximity();
		
		if (randForPosOrNeg == 1) {
			coordinateX[i] = -randomCoordinate;
		}
		else coordinateX[i] = randomCoordinate;
		
		randForPosOrNeg = rand() % 2 + 1;
		
		if (randForPosOrNeg == 1) {
			coordinateY[i] = -randomCoordinate;
		}
		else coordinateY[i] = randomCoordinate;

		enemyActive[i] = true;
		enemySize[i] = rand() % 2 + 1;

		GLint randType = rand() % 4 + 1;
		enemyType[i] = randType;
		
		for (int i = 0; i < red.size(); i++)
		{
			red[i] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
		}
		for (int i = 0; i < green.size(); i++)
		{
			green[i] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
		}
		for (int i = 0; i < blue.size(); i++)
		{
			blue[i] = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
		}
		
		
	}
	
}


GLint proximity() {             //gets a random number that is not on the center where the ship is, or close to it

	GLint randomNum = rand() % 16 - 8;

	while (randomNum <= 2 && randomNum >= -2) {
		randomNum = rand() % 16 - 8;
	}

	return randomNum;
}
void checkIfLasersHasExpired(lasers bullets) {

	if (bullets.active && (bullets.laserX > 10|| bullets.laserX < -10 || bullets.laserY > 10 || bullets.laserY < -10)) {
		bullets.active = false;
	}
}
void writeString(void *font, char *string)
{
	char *a;
	for (a = string; *a != '\0'; a++) glutBitmapCharacter(font, *a);
}
void reshape(int width, int height)
{
	glViewport(0, -0, width, height);
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();
	/* uses orthographic (parallel) projection
	use xmin = -1, xmax = 1
	ymin = -1, ymax = 1
	znear = -1, zfar = 1 - not relevant here (2D)*/
	glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);   //sets the x,y,z plane from -1 to 1

	glMatrixMode(GL_MODELVIEW);
}
void drawSquare(GLfloat length, GLfloat x, GLfloat y)
{
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat x1, y1, x2, y2, x3, y3, x4, y4;

	glColor3f(0.0, 0.0, 0.5);
	x1 = x - length / 2;
	y1 = y + length / 2;
	x2 = x + length / 2;
	y2 = y + length / 2;
	x3 = x + length / 2;
	y3 = y - length / 2;
	x4 = x - length / 2;
	y4 = y - length / 2;

	
	glBegin(GL_POLYGON);
	
	glVertex2f(x1, y1);
	//glVertex2f(x1 - length / 2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	
	glVertex2f(x4, y4);
	glEnd();

	//glFlush();

	
}
void drawObject(GLfloat length, GLfloat x, GLfloat y) {
	GLfloat x1, y1, x2, y2, x3, y3, x4, y4;

	glColor3f(0.0, 0.3, 0.0);
	x1 = x - length / 2;
	y1 = y + length / 2;
	x2 = x + length / 2;
	y2 = y + length / 2;
	x3 = x + length / 2;
	y3 = y - length / 2;
	x4 = x - length / 2;
	y4 = y - length / 2;


	glBegin(GL_POLYGON);
	glVertex2f(x1, y1);
	
	glVertex2f(x2, y2);
	
	glVertex2f(x3, y3);
	glVertex2f(x1 + length / 2, y1 + 0.2);
	glVertex2f(x4, y4);
	glEnd();
	//glFlush();
	
}
void drawShip() 
{
	glColor3f(0.5, 0.0, 0.0);
	
	glBegin(GL_POLYGON);

    glVertex2f(shipX - shipY, shipY - shipSize / 4);
	glVertex2f(shipX - shipSize / 3, shipY - shipSize / 2);
	glVertex2f(shipX - shipY, shipY + shipSize / 2);
	glVertex2f(shipX + shipSize / 3, shipY - shipSize / 2);
	

	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex2f(shipX - shipY, shipY - shipSize / 4);
	glVertex2f(shipX - shipSize / 3, shipY - shipSize / 2);
	glVertex2f(shipX - shipY, shipY + shipSize / 2);
	glVertex2f(shipX + shipSize / 3, shipY - shipSize / 2);
	glEnd();

	//glFlush();
}
void drawTriangle(GLfloat length, GLfloat x, GLfloat y) {
	
	glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_POLYGON);

	
	glVertex2f(x - length / 3, y - length / 2);
	glVertex2f(x, y + length / 2);
	glVertex2f(x + length / 3, y - length / 2);


	glEnd();
	//glFlush();
}
void drawStar(GLfloat radius, GLfloat x, GLfloat y,GLfloat red, GLfloat green, GLfloat blue)
{
	//x1,y1 is the first coordinate at 72 degrees from the unit circle

	

	glColor3f(red,green,blue);
	GLfloat x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;

	x1 = x + cos(72 * PI / 180) * radius;
	y1 = y + sin(72 * PI / 180) * radius;
	x2 = x + cos(144 * PI / 180) * radius;
	y2 = y + sin(144 * PI / 180) * radius;
	x3 = x + cos(216 * PI / 180) * radius;
	y3 = y + sin(216 * PI / 180) * radius;
	x4 = x + cos(288 * PI / 180) * radius;
	y4 = y + sin(288 * PI / 180) * radius;
	x5 = x + radius;
	y5 = y;


	glLineWidth(1.0);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x3, y3);
	glVertex2f(x1, y1);
	glVertex2f(x4, y4);
	glVertex2f(x2, y2);
	glVertex2f(x4, y4);
	glVertex2f(x2, y2);
	glVertex2f(x5, y5);
	glVertex2f(x3, y3);
	glVertex2f(x5, y5);
	glEnd();

	
}
void createLazer() 
{
	
	laser = true;
    lasers shoot = lasers(shipX, shipY,true,angle);
 	shoot.travelX = shoot.missleSpeed * -sin(shoot.laserAngle* (PI/ 180));
	shoot.travelY = shoot.missleSpeed*  cos(shoot.laserAngle*  (PI / 180));
 	
	laserManager.push_back(shoot);
	
}
void drawCircle(GLfloat radius, GLfloat x, GLfloat y)
{
	GLint i;
	GLint numLines = 1000;   //number of lines

	glLineWidth(2.0);
	//glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	for (i = 0; i <= numLines; i++)
	{
		glVertex2f(x + radius * cos(i * angle / numLines), y + radius * sin(i * angle / numLines));
	}
	glEnd();

}
void drawLaser(lasers shoot) {
	
	
	glColor3f(0.5, 0.0, 0.0);
	glPointSize(6.0);
	glLoadIdentity();
	glPushMatrix();

	glTranslatef(shoot.laserX, shoot.laserY, 0.0);
	glRotatef(shoot.laserAngle, 0.0, 0.0, 1.0);
	glTranslatef(-shoot.laserX, -shoot.laserY, 0.0);

	drawCircle(0.1f, shoot.laserX, shoot.laserY);
	
	glPopMatrix();
	
}
void moveLaser() {


 	for (int i = 0; i < laserManager.size(); i++)
	{
		if (laserManager[i].active)
		{
						
			laserManager[i].laserX = laserManager[i].laserX + laserManager[i].travelX/2;
			laserManager[i].laserY = laserManager[i].laserY + laserManager[i].travelY/2;
		}
		else laserManager.erase(laserManager.begin() + i);

	}
	
    
		for (int i = 0; i < laserManager.size(); i++)
		{
				checkIfLasersHasExpired(laserManager[i]);	
		}		

}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	displayBackground();

	if (enemyCount == 0)
	{
		populate();            //initializes the components of the enemies
		enemyCount = 4;
		laserManager.clear();  //just to kill any bullet that might be flying when a new set of stars appear
	}

	for (int i = 0; i < enemyActive.size(); i++)   //draws the enemies
	{
		if (enemyActive[i]) {

			if (enemyAngle > 360)
			{
				enemyAngle = 0;
			}

			if (enemyAngle < 0)
			{
				enemyAngle = 360;
			}


			GLint random = rand() % 3;
			glLoadIdentity();

			glPushMatrix();
			glTranslatef(coordinateX[i], coordinateY[i], 0.0);
			glRotatef(enemyAngle, 0.0, 0.0, 1.0);
			glTranslatef(-coordinateX[i], -coordinateY[i], 0.0);

			if (enemyType[i] == 1)
			{
				drawStar(2.0, coordinateX[i], coordinateY[i],red[i],green[i],blue[i]);
			}
			else if (enemyType[i] == 2)
			{

				drawStar(1.5, coordinateX[i], coordinateY[i], red[i], green[i], blue[i]);
			}
			else if (enemyType[i] == 3)
			{
				drawStar(1.0, coordinateX[i], coordinateY[i], red[i], green[i], blue[i]);
			}
			else if (enemyType[i] == 4)
			{
				drawStar(2.5, coordinateX[i], coordinateY[i], red[i], green[i], blue[i]);
			}
		}
		//enemyAngle += 10;
		glPopMatrix();
		//glFlush();

		for (int i = 0; i < laserManager.size(); i++)             //this section checks every active laser and sees if it has collided with an asteriod
		{
			if (laserManager[i].active)
			{
				for (int b = 0; b < enemyActive.size(); b++)
				{
					
					if (enemyActive[b])                        //also checks if enemies is active so the lasers wont check with the coordinates of an invisible primitive
					{
						GLfloat starRadius = 2.0;

						if (enemyType[b] == 1)
						{
							starRadius = 2.0;
						}
						else if (enemyType[b] == 2)
						{
							starRadius = 1.5;
						}
						else if (enemyType[b] == 3)
						{
							starRadius = 1.0;
						}
						else if (enemyType[b] == 4)
						{
							starRadius = 2.5;
						}
						
						
						if (detectCollisions(starRadius, coordinateX[b], coordinateY[b], laserManager[i].laserX, laserManager[i].laserY)) {
							laserManager[i].active = false;
							enemyActive[b] = false;
						}
					}
					
				}
			}

		}
	}

	

	glLoadIdentity();

	glPushMatrix();
	glTranslatef(shipX, shipY, 0.0);
	glRotatef(angle, 0.0, 0.0, 1.0);    //shipRotation
	glTranslatef(-shipX, -shipY, 0.0);

	drawShip();
	glPopMatrix();

	glLoadIdentity();
	glPushMatrix();

	for (int i = 0; i < laserManager.size(); i++)
	{
		if (laserManager[i].getActive())
		{
			drawLaser(laserManager[i]);
		}
	}
	glPopMatrix();





	glFlush();


	//write "score" + the achieved score
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(-9, 9);
	writeString(GLUT_BITMAP_TIMES_ROMAN_24, "SCORE: ");

	char score[10] = { '\0' };
	sprintf_s(score, "%d", myScore);
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(-6, 9);
	writeString(GLUT_BITMAP_TIMES_ROMAN_24, score);

	glutSwapBuffers();
}
void playerInput() 
{
	if (angle > 360)
	{
		angle = 0;
	}

	if (angle < 0)
	{
		angle = 360;
	}

	
	if(leftKeyPressed)
	{
		angle = angle + 10;
	}
	else if(rightKeyPressed)
	{
		angle = angle - 10;
	}

	if(spaceBarPressed)
	{
	 	createLazer();
	}
}
void timer(int value) {
	glClear(GL_COLOR_BUFFER_BIT);
	
	playerInput();
	enemyAngle += 10;
	moveLaser();
	glutPostRedisplay();
	glutTimerFunc(60, timer, 0);
}

void spaceKeyPressed(unsigned char key, GLint x, GLint y)
{
	
 	if(key +' ')
	{
		spaceBarPressed = true;
	}	
	//createLazer();
}

void spaceKeyRelease(unsigned char key, GLint x, GLint y)
{
	
	if (key = ' ') {
		spaceBarPressed = false;
	}
	
}
void arrowKeyDown(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		leftKeyPressed = true;

	}
	if (key == GLUT_KEY_RIGHT)
	{
		rightKeyPressed = true;

	}

	if (key == GLUT_KEY_UP)
	{

		upKeyPressed = true;
	}

	if (key == GLUT_KEY_DOWN)
	{
		downKeyPressed = true;
	}


}
void arrowKeyReleased(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
	{
		upKeyPressed = false;
		break;
	}
	case GLUT_KEY_DOWN:
	{
		downKeyPressed = false;
		break;
	}
	case GLUT_KEY_LEFT:
	{
		leftKeyPressed = false;
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		rightKeyPressed = false;
		break;
	}

	}
}

void init(void)
{
	
	glClearColor(0.0, 0.0, 0.0, 0.0);     /* window will be cleared to red */
    myShip = new ship();
}

int main(int argc, char** argv)
{
	/* window management code ... */
	/* initialises GLUT and processes any command line arguments */
	glutInit(&argc, argv);
	/* use single-buffered window and RGBA colour model */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	/* window width = 400 pixels, height = 400 pixels */
	/* window width = 640 pixels, height = 480 pixels for a 4:3 ascpect ratio */
	/* window width = 1024 pixels, height = 576 pixels for a 16:9 ascpect ratio */
	glutInitWindowSize(800, 800);
	/* window upper left corner at (100, 100) */
	glutInitWindowPosition(500, 200);
	/* creates an OpenGL window with command argument in its title bar */
	glutCreateWindow("Asteriods");

	init();
	cout << "Welcome to My Coursework: ASTERIODS" << endl << "Instructions are Simple!\n\n\n" << endl << "[1] press 'left arrow key' to rotate left\n\n\n" << "[2]press 'right arrow key' to rotate right\n\n\n" << "[3] press 'spacebar' to FIRE! \n\n\n" << "[4] Right Mouse Click to open an option Menu :D \n\n\n" << "[6] ENJOY!!";
	
	glutCreateMenu(createMenu);
	glutAddMenuEntry("Grid On", 1);
	glutAddMenuEntry("Pattern On", 2);
	glutAddMenuEntry("No Pattern", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	glutSpecialFunc(arrowKeyDown);
	glutSpecialUpFunc(arrowKeyReleased);
	glutKeyboardFunc(spaceKeyPressed);
	glutKeyboardUpFunc(spaceKeyRelease);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(5, timer, 0);
	glutMainLoop();
	return 0;
}