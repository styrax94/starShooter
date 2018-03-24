#include "lasers.h"



lasers::lasers(GLfloat x, GLfloat y, GLboolean a,GLfloat b)
{
	laserX = x;
	laserY = y;
	active = a;
	laserAngle = b;
}

GLfloat lasers::getX() {
	return laserX;
}

GLfloat lasers::getY() {
	return laserY;
}

GLboolean lasers::getActive() {
	return active;
}
GLfloat lasers::getSpeed() {
	return missleSpeed;
}
GLfloat lasers::getTravelX() {
	return travelX;
}
GLfloat lasers::getTravelY() {
	return travelY;
}

GLfloat lasers::getLaserAngle() {
	return laserAngle;
}
void lasers::setActive() {
	active = false;
}

void lasers::setTravelX(GLfloat x) {
	travelX = x;
}

void lasers::setTravelY(GLfloat y) {
	travelY = y;
}
void lasers::setLaserX(GLfloat x) {
	travelX = travelX + x;
}
void lasers::setLaserY(GLfloat y) {
	travelY = travelY + y;
}



