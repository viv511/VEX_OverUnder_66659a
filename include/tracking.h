#include "main.h"
#include "waypoint.h"

#pragma once

using namespace pros;

void initializeTracking();
void tracking();

float avgDist();

Waypoint getCurrentPose();

float distDiff(float startX, float startY, float endX, float endY);
float angDiff(float startX, float startY, float endX, float endY);
void set();