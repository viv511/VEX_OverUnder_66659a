#include "waypoint.h"
#include <cmath>

using namespace pros;

constexpr float PI = 3.141592;
constexpr float degreesToRadians = PI/180;
constexpr float radiansToDegrees = 180/PI;

Waypoint::Waypoint(float xCoord, float yCoord) {
    this->x = xCoord;
    this->y = yCoord;
    this->theta = 0;
    this->dist = 0;
    this->vel = 0;
    this->curv = 0;
}

Waypoint::Waypoint(float xCoord, float yCoord, float angleTheta) {
    this->x = xCoord;
    this->y = yCoord;
    this->theta = angleTheta;
    this->dist = 0;
    this->vel = 0;
    this->curv = 0;
}

float Waypoint::getX() {
    return this->x;
}

float Waypoint::getY() {
    return this->y;
}

float Waypoint::getTheta() {
    return this->theta;
}

float Waypoint::getDist() {
    return this->dist;
}

float Waypoint::getVel() {
    return this->vel;
}

float Waypoint::getCurv() {
    return this->curv;
}

void Waypoint::setX(float newX) {
    this->x = newX;
}

void Waypoint::setY(float newY) {
    this->y = newY;
}

void Waypoint::setTheta(float newTheta) {
    this->theta = newTheta;
}

void Waypoint::setDist(float newDist) {
    this->dist = newDist;
}

void Waypoint::setVel(float newVel) {
    this->vel = newVel;
}

void Waypoint::setCurv(float newCurv) {
    this->curv = newCurv;
}

Waypoint scalarMult(Waypoint P, float s) {
	Waypoint Ps = Waypoint(P.getX()*s, P.getY()*s);
	return Ps;
}

float distance(Waypoint A, Waypoint B) {
	Waypoint originVec = Waypoint(B.getX()-A.getX(), B.getY()-A.getY());
	return getLength(originVec);
}

float angle(Waypoint A, Waypoint B) {
	float dTheta = std::atan2(B.getY() - A.getY(), B.getX() - A.getX());

	return dTheta*radiansToDegrees;
}

Waypoint normalizeVect(Waypoint P) {
	float len = getLength(P);
	Waypoint U = Waypoint(P.getX()/len, P.getY()/len);
	return U;
}

Waypoint getDirVector(Waypoint A, Waypoint B) {
	return Waypoint(B.getX()-A.getX(), B.getY()-A.getY());
}

float dotProduct(Waypoint A, Waypoint B) {
	float Dot = A.getX() * B.getX() + A.getY() * B.getY();
	return Dot;
}

float getLength(Waypoint P) {
	float pointX = P.getX();
	float pointY = P.getY();
	if(!((pointX == 0) && (pointY == 0))) {
		return std::sqrt(pow(pointX, 2) + pow(pointY, 2));
	}
	else {
		return 0;
	}
}

bool areSame(Waypoint A, Waypoint B) {
    if(A.getX() == B.getX()) {
        if(A.getY() == B.getY()) {
            return true;
        }
    }

    return false;
}

Waypoint lerp(Waypoint A, Waypoint B, float frac) {
    float newX = A.getX() + (B.getX() - A.getX()) * frac;
    float newY = A.getY() + (B.getY() - A.getY()) * frac;

    Waypoint newPoint = Waypoint(newX, newY, A.getTheta());
    return newPoint;
}
