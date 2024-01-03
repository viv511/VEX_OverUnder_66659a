#include "main.h"
#include "waypoint.h"

#pragma once

using namespace pros;

void initializeTracking();
void tracking();

float avgDist();

Waypoint getCurrentPose();

void set();