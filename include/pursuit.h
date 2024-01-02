#include "main.h"

#ifndef PURSUIT_H
#define PURSUIT_H

using namespace pros;

void pathFollowPurePursuit(vector<Waypoint> pathToFollow, float lookaheadRadius, bool fwd, float maxVel, float maxA, float velocityK);

vector<Waypoint> pathGen(vector<Waypoint> pathToFollow, float maxVel, float maxA, float velocityK);
vector<Waypoint> smooth(vector<Waypoint> roughPath, float a, float b, float tolerance);

Waypoint findClosestPoint(Waypoint P,  vector<Waypoint> path);
float circleLineIntersect(Waypoint start, Waypoint end, Waypoint curPos, float lookaheadRadius);
Waypoint findLookaheadPoint(vector<Waypoint> pathToFollow, Waypoint curPos, Waypoint prevLookAhead, float lookaheadRadius);
float getSignedCurvature(Waypoint curPos, Waypoint lookAhead, float orientation);

#endif