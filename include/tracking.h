#include "main.h"
#include "waypoint.h"

#pragma once

using namespace pros;

void initializeTracking();
void tracking();

float avgDist();

Waypoint getCurrentPose();
void setStart(float sX, float sY);

void set();