#include "main.h"

#pragma once

using namespace pros;

void initializeTracking();
void tracking();
std::pair<float, float> getDist();
float avgDist();
std::pair<float, float> getCoords();
float distDiff(float startX, float startY, float endX, float endY);
float angDiff(float startX, float startY, float endX, float endY);
