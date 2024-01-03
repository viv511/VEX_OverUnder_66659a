#include "pursuit.h"
#include "waypoint.h"
#include "tracking.h"
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

const int TIME_INTERVAL = 10;
constexpr float PI = 3.14159265358979323846;

// maxVel - print out the derivative of odometry
// maxA - print out the inertial.get_accel() and take max
// k - constant from 1 to 5

constexpr float lookaheadDist = 10;
constexpr float trackWidth = 1000000; //VIVEK NEED TO TUNE!!
constexpr float kV = 0.0055; //VIVEK NEED TO TUNE!!
constexpr float kA = 0.002; //VIVEK NEED TO TUNE!!
constexpr float kP = 0.01; //VIVEK NEED TO TUNE!!

std::vector<Waypoint> path = {{1, 1}, {100, 100}, {300, 50}, {500, 200}};

//Inspo DAWGMA + LemLib
void pathFollowPurePursuit(vector<Waypoint> pathToFollow, float lookaheadRadius, bool fwd, float maxVel, float maxA, float velocityK) {
    
    pathToFollow = pathGen(pathToFollow, maxVel, maxA, velocityK);
    
    Waypoint currentPos = getCurrentPose();
    Waypoint lastPos = currentPos;
    Waypoint lookPos = Waypoint(0, 0, 0);
    Waypoint lastLookPos = pathToFollow[0];

    Waypoint closestPoint = Waypoint(0, 0, 0);
    
    float currentCurvature;
    float targetVel;
    float prevLeftVel = 0;
    float prevRightVel = 0;

    for(int i=0; i<pathToFollow.size();  i++) {
        currentPos = getCurrentPose();

        // if(!fwd) {
        //     currentPos.setTheta(currentPos.getTheta() - PI);
        // }

        //update dist travelled for our weird task 
        lastPos = currentPos;

        closestPoint = findClosestPoint(currentPos, pathToFollow);
        if(areSame(closestPoint, pathToFollow[pathToFollow.size()-1])) { //REACHED THE END
            break;
        }

        lookPos = findLookaheadPoint(pathToFollow, currentPos, lastLookPos, lookaheadRadius);
        lastLookPos = lookPos;
         
        currentCurvature = getSignedCurvature(currentPos, lookPos, currentPos.getTheta());

        targetVel = closestPoint.getVel(); //maybe RATE LIMITER OR SMTH HERE

        //Velocity Calculations: **************************************************
        //L = V * (2 + CT) / 2
        //R = V * (2 - CT) / 2
        float leftVel = targetVel * (2 + currentCurvature * trackWidth) / 2;
        float rightVel = targetVel * (2 - currentCurvature * trackWidth) / 2;
        
        float leftTargetAccel = (leftVel - prevLeftVel) / TIME_INTERVAL;
        float rightTargetAccel = (rightVel - prevRightVel) / TIME_INTERVAL;

        prevLeftVel = leftVel;
        prevRightVel = rightVel;
        //*************************************************************************

        //Feedforward calculations:
        float ffLeft = kV * leftVel + kA * leftTargetAccel;
        float ffRight = kV * rightVel + kA * rightTargetAccel;

        float fbLeft = kP * (leftVel - getLeftVel());
        float fbRight = kP * (rightVel - getRightVel());

        float actualLeft = ffLeft + fbLeft;
        float actualRight = ffRight + fbRight;

        //Find max and ratio it
        float biggest = max(fabs(actualLeft), fabs(actualRight));
        actualLeft = actualLeft / biggest * 12000;
        actualRight = actualRight / biggest * 12000;

        if(fwd) {
            LeftDT.move_voltage(actualLeft);
            RightDT.move_voltage(actualRight);
        }
        else {
            LeftDT.move_voltage(-actualLeft);
            RightDT.move_voltage(-actualRight);
        }

        pros::delay(TIME_INTERVAL);

    }

    stopMotors();

}

vector<Waypoint> readPoints(string fileName) {
    vector<Waypoint> points;
    ifstream file(fileName);
    string line;
    while(getline(file, line)) {
        stringstream ss(line);
        string x, y;
        getline(ss, x, ',');
        getline(ss, y, ',');
        points.push_back(Waypoint(stof(x), stof(y)));
    }
    return points;
}

vector<Waypoint> pathGen(vector<Waypoint> pathToFollow, float maxVel, float maxA, float velocityK) {
    //Following DAWGMA Document

    // // Step 1. Injecting extra points
    // int inchSpacing = 6;
    // vector<Waypoint> newPath;
    // for(int lineSeg = 0; lineSeg < pathToFollow.size()-1; lineSeg++) {
    //     Waypoint dirVector = getDirVector(pathToFollow[lineSeg], pathToFollow[lineSeg+1]); 
    //     int totalPointsFit = (int) (getLength(dirVector) / inchSpacing);
    //     dirVector = scalarMult(normalizeVect(dirVector), inchSpacing);

    //     //Inject points
    //     for(int i=0; i<totalPointsFit; i++) {
    //         newPath.push_back(Waypoint(pathToFollow[lineSeg].getX() + scalarMult(dirVector, i).getX(), pathToFollow[lineSeg].getY() + scalarMult(dirVector, i).getY()));
    //     }
    // }
    // newPath.push_back(Waypoint(pathToFollow[pathToFollow.size()-1].getX(), pathToFollow[pathToFollow.size()-1].getY()));

    // //Step 2. Smooth Path
    // newPath = smooth(newPath, 0.3, 0.7, 0.001);

    //Step 3. Distance between points
    pathToFollow[0].setDist(0);
    for(int i=1; i<pathToFollow.size(); i++) {
        //Runing Sum (D_i = D_i-1 + dist(D_i, D_i-1)
        pathToFollow[i].setDist((pathToFollow[i-1].getDist() + distance(pathToFollow[i], pathToFollow[i-1])));
    }

    //Step 4. Calculate curvature (1/radius) between points
    pathToFollow[0].setCurv(0);
    for(int i=1; i<(pathToFollow.size()-1); i++) {
        float x1 = pathToFollow[i-1].getX();
        float y1 = pathToFollow[i-1].getY();
        float x2 = pathToFollow[i].getX();
        float y2 = pathToFollow[i].getY();
        float x3 = pathToFollow[i+1].getX();
        float y3 = pathToFollow[i+1].getY();

        if(x1 == y1) {
            //Account for divide by 0 edge cases
            pathToFollow[i-1].setX(x1 + 0.001);
        }

        float kOne = 0.5 * (pow(x1, 2) + pow(y1, 2) - pow(x2, 2) - pow(y2, 2)) / (x1 - x2);
        float kTwo = (y1 - y2) / (x1 - x2);

        float b = 0.5 * (pow(x2, 2) - 2 * x2 * kOne + pow(y2, 2) - pow(x3, 2) + 2 * x3 * kOne - pow(y3, 2)) / (x3 * kTwo - y3 + y2 - x2 * kTwo);
        float a = kOne - kTwo * b;

        float r = sqrt(pow((x1 - a), 2) + pow((y1 - b), 2));
        float c = 1/r;

        if(std::isnan(c)) {
            //Straight line
            pathToFollow[i].setCurv(0);
        }
        else {
            pathToFollow[i].setCurv(c);
        }
    }
    pathToFollow[pathToFollow.size()-1].setCurv(0);

    //Step 5a. Calculate Velocities
    for(int i=0; i<pathToFollow.size(); i++) {
        if(pathToFollow[i].getCurv() == 0) {
            pathToFollow[i].setVel(maxVel);
        }
        else {
            pathToFollow[i].setVel(min(velocityK/(pathToFollow[i].getCurv()), maxVel));
        }
    }

    //Step 5b & c
    //i and i+1 becomes i-1 and i
    pathToFollow[pathToFollow.size()-1].setVel(0);
    for(int i=pathToFollow.size()-1; i>0; i--) {
        float dist = distance(pathToFollow[i], pathToFollow[i-1]);
        float newVel = sqrt(pow(pathToFollow[i].getVel(), 2) + 2 * maxA * dist);

        pathToFollow[i-1].setVel(min(pathToFollow[i-1].getVel(), newVel));
    }


    return pathToFollow;
}

vector<Waypoint> smooth(vector<Waypoint> roughPath, float a, float b, float tolerance) {
    //a == weight on the data, b == weight for smoothing"
    //A larger "b" value will result in a smoother path, be careful!
    vector<Waypoint> smoothPath;
    for(int index = 0; index < roughPath.size(); index++) {
        smoothPath.push_back(roughPath[index]);
    }
    
    double change = tolerance;
	while(change >= tolerance) {
		change = 0.0;
		for(int i=1; i<roughPath.size()-1; i++) {
            double tempX = smoothPath[i].getX();
            smoothPath[i].setX(smoothPath[i].getX() + (a * (roughPath[i].getX() - smoothPath[i].getX()) + b * (smoothPath[i-1].getX() + smoothPath[i+1].getX() - (2.0 * smoothPath[i].getX()))));
            change += abs(tempX - smoothPath[i].getX());

            double tempY = smoothPath[i].getY();
            smoothPath[i].setY(smoothPath[i].getY() + (a * (roughPath[i].getY() - smoothPath[i].getY()) + b * (smoothPath[i-1].getY() + smoothPath[i+1].getY() - (2.0 * smoothPath[i].getY()))));
            change += abs(tempY - smoothPath[i].getY());
		}
	}

    return smoothPath;
    //Credit ~Team 2168 FRC/FTC for smoothing algorithm
}

Waypoint findClosestPoint(Waypoint P, vector<Waypoint> path) {
    //Returns index of closest point in path to point P
    float smallestDist = 10000000;
    int smallestIndex = -1;

    float dist = smallestDist;
    for(int i=0; i<path.size(); i++) {
        dist = distance(P, path[i]);
        if(dist < smallestDist) {
            smallestDist = dist;
            smallestIndex = i;
        }
    }

    return path[smallestIndex];
}

float circleLineIntersect(Waypoint start, Waypoint end, Waypoint curPos, float lookaheadRadius) {
    //Returns t value of intersection between Circle and Line Segment [formed by "start" & "end"] (-1 if none)
    Waypoint dirVect = getDirVector(start, end);
    Waypoint centerVect = getDirVector(curPos, start);

    float a = dotProduct(dirVect, dirVect);
    float b = 2 * dotProduct(centerVect, dirVect);
    float c = dotProduct(centerVect, centerVect) - lookaheadRadius * lookaheadRadius;

    //Calculate discriminant: b^2-4ac
    float d = b * b - 4 * a * c;

    if(d >= 0) {
        d = sqrt(d);
        float t1 = (-b - d) / (2 * a);
        float t2 = (-b + d) / (2 * a);
        if(t1 >= 0 && t1 <= 1) {
            return t1;
        }
        else if(t2 >= 0 && t2 <= 1) {
            return t2;
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }


   //https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm/1084899#1084899
}

Waypoint findLookaheadPoint(vector<Waypoint> pathToFollow, Waypoint curPos, Waypoint prevLookAhead, float lookaheadRadius) {
    //Uses LemLib's optimizations
    int lastIndex = -1;
    for(int i=0; i<pathToFollow.size(); i++) {
        if(areSame(prevLookAhead, pathToFollow[i])) {
            lastIndex = i;
        }
    }

    //Reverse Search
    for(int i = pathToFollow.size()-1; i >= lastIndex; i--) {
        //Due to RevSearch, need to compare current and previous
        Waypoint cur = pathToFollow[i];
        Waypoint prev = pathToFollow[i-1];

        float fracValue_t = circleLineIntersect(cur, prev, curPos, lookaheadRadius);

        if(fracValue_t != -1) {
            return lerp(prev, cur, fracValue_t);
        }   
    }

    return prevLookAhead; //Else failed
}

float getSignedCurvature(Waypoint curPos, Waypoint lookAhead, float orientation) {
    //Signed Curvature = curvature * side
    float robotAngle = curPos.getTheta(); 

    //Curvature = 2x/L^2 (use DAWGMA document)
    float a = -std::tan(robotAngle);
    float c = std::tan(robotAngle) * curPos.getX() - curPos.getY();
    float L = distance(curPos, lookAhead);

    //x = |ax+by+c| / sqrt(a^2+b^2), b = 1
    //x = |ax + y + c| / sqrt(a^2 + 1)
    float x = std::fabs(a * lookAhead.getX() + lookAhead.getY() + c) / std::sqrt(pow(a, 2) + 1);
    float curvature = ((2 * x) / pow(L, 2));

    float side;
    (std::sin(robotAngle) * (lookAhead.getX()-curPos.getX()) - std::cos(robotAngle) * (lookAhead.getY()-curPos.getY()) >= 0) ? side = 1 : side = -1;

    return curvature * side;
}

float getRightVel() {
    return (FR.get_actual_velocity()+MR.get_actual_velocity()+BR.get_actual_velocity())/3;
}

float getLeftVel() {
    return (FL.get_actual_velocity()+ML.get_actual_velocity()+BL.get_actual_velocity())/3;
}