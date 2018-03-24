#pragma once
#include "..\include\freeglut.h"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;


class lasers
{

private:
	
	
public:
	 lasers(GLfloat x, GLfloat y,GLboolean a,GLfloat b);
	 GLfloat getX();
	 GLfloat getY();
	 GLboolean getActive();
	 GLfloat getSpeed();
	 GLfloat getTravelX();
	 GLfloat getTravelY();
	 GLfloat getLaserAngle();
	 void setActive();
	 void setTravelX(GLfloat x);
	 void setTravelY(GLfloat y);
	 void setLaserX(GLfloat x);
	 void setLaserY(GLfloat y);
	 GLfloat missleSpeed = 0.5, laserX, laserY, travelX, travelY, laserAngle;
	 GLboolean active;
};