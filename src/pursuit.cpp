#include "pursuit.h"
#include "waypoint.h"
#include "tracking.h"
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

const int TIME_INTERVAL = 10;
constexpr float PI = 3.14159265358979323846;

constexpr float maximumVelocity = 0.09; //Solved
constexpr float maximumAcceleration = 0.002; //Solved
constexpr float constantK = 2; //Needs to tune
constexpr float lookaheadDist = 10; //Need to tune
constexpr float trackWidth = 11.75;
constexpr float kV = 0.0055; //VIVEK NEED TO TUNE!!
constexpr float kA = 0.002; //VIVEK NEED TO TUNE!!
constexpr float kP = 0.01; //VIVEK NEED TO TUNE!!


void followRoute(string routeName, bool isfwd) {
    vector<Waypoint> path = readPoints(routeName);

    // for(int i=0; i<path.size(); i++) {
    //     std::cout << path[i].getX() << "\t" << path[i].getY() << "\t" << path[i].getVel() << "\n";
    // }

    // cout << path.size();

    // controller.rumble(".....");

    pathFollowPurePursuit(path, lookaheadDist, isfwd, maximumVelocity, maximumAcceleration, constantK);
}

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

    vector<Waypoint> path;
Waypoint newPoint = Waypoint(0, 0, 0);
newPoint.setX(23.8195);
newPoint.setY(-33.45);
newPoint.setVel(1);
path.push_back(newPoint);
newPoint.setX(23.6263);
newPoint.setY(-32.7195);
newPoint.setVel(1.00292);
path.push_back(newPoint);
newPoint.setX(23.4253);
newPoint.setY(-31.9889);
newPoint.setVel(1.00573);
path.push_back(newPoint);
newPoint.setX(23.217);
newPoint.setY(-31.2581);
newPoint.setVel(1.00843);
path.push_back(newPoint);
newPoint.setX(23.0019);
newPoint.setY(-30.5272);
newPoint.setVel(1.01098);
path.push_back(newPoint);
newPoint.setX(22.7807);
newPoint.setY(-29.7962);
newPoint.setVel(1.01338);
path.push_back(newPoint);
newPoint.setX(22.5538);
newPoint.setY(-29.0652);
newPoint.setVel(1.0156);
path.push_back(newPoint);
newPoint.setX(22.3219);
newPoint.setY(-28.334);
newPoint.setVel(1.01763);
path.push_back(newPoint);
newPoint.setX(22.0854);
newPoint.setY(-27.6028);
newPoint.setVel(1.01947);
path.push_back(newPoint);
newPoint.setX(21.8449);
newPoint.setY(-26.8715);
newPoint.setVel(1.02109);
path.push_back(newPoint);
newPoint.setX(21.6009);
newPoint.setY(-26.1401);
newPoint.setVel(1.02249);
path.push_back(newPoint);
newPoint.setX(21.354);
newPoint.setY(-25.4087);
newPoint.setVel(1.02367);
path.push_back(newPoint);
newPoint.setX(21.1048);
newPoint.setY(-24.6772);
newPoint.setVel(1.0246);
path.push_back(newPoint);
newPoint.setX(20.8538);
newPoint.setY(-23.9458);
newPoint.setVel(1.0253);
path.push_back(newPoint);
newPoint.setX(20.6014);
newPoint.setY(-23.2142);
newPoint.setVel(1.02575);
path.push_back(newPoint);
newPoint.setX(20.3484);
newPoint.setY(-22.4827);
newPoint.setVel(1.02596);
path.push_back(newPoint);
newPoint.setX(20.0952);
newPoint.setY(-21.7512);
newPoint.setVel(1.02592);
path.push_back(newPoint);
newPoint.setX(19.8423);
newPoint.setY(-21.0197);
newPoint.setVel(1.02563);
path.push_back(newPoint);
newPoint.setX(19.5903);
newPoint.setY(-20.2882);
newPoint.setVel(1.0251);
path.push_back(newPoint);
newPoint.setX(19.3398);
newPoint.setY(-19.5567);
newPoint.setVel(1.02432);
path.push_back(newPoint);
newPoint.setX(19.0913);
newPoint.setY(-18.8253);
newPoint.setVel(1.0233);
path.push_back(newPoint);
newPoint.setX(18.8454);
newPoint.setY(-18.0939);
newPoint.setVel(1.02205);
path.push_back(newPoint);
newPoint.setX(18.6025);
newPoint.setY(-17.3625);
newPoint.setVel(1.02057);
path.push_back(newPoint);
newPoint.setX(18.3633);
newPoint.setY(-16.6313);
newPoint.setVel(1.01888);
path.push_back(newPoint);
newPoint.setX(18.1282);
newPoint.setY(-15.9);
newPoint.setVel(1.01698);
path.push_back(newPoint);
newPoint.setX(17.8979);
newPoint.setY(-15.1689);
newPoint.setVel(1.01488);
path.push_back(newPoint);
newPoint.setX(17.6729);
newPoint.setY(-14.4379);
newPoint.setVel(1.0126);
path.push_back(newPoint);
newPoint.setX(17.4536);
newPoint.setY(-13.7069);
newPoint.setVel(1.01015);
path.push_back(newPoint);
newPoint.setX(17.2408);
newPoint.setY(-12.9761);
newPoint.setVel(1.00754);
path.push_back(newPoint);
newPoint.setX(17.0348);
newPoint.setY(-12.2454);
newPoint.setVel(1.00481);
path.push_back(newPoint);
newPoint.setX(16.8363);
newPoint.setY(-11.5148);
newPoint.setVel(1.00196);
path.push_back(newPoint);
newPoint.setX(16.6458);
newPoint.setY(-10.7843);
newPoint.setVel(0.999009);
path.push_back(newPoint);
newPoint.setX(16.4639);
newPoint.setY(-10.054);
newPoint.setVel(0.995992);
path.push_back(newPoint);
newPoint.setX(16.291);
newPoint.setY(-9.32384);
newPoint.setVel(0.992928);
path.push_back(newPoint);
newPoint.setX(16.1278);
newPoint.setY(-8.59384);
newPoint.setVel(0.989846);
path.push_back(newPoint);
newPoint.setX(15.9748);
newPoint.setY(-7.86402);
newPoint.setVel(0.986773);
path.push_back(newPoint);
newPoint.setX(15.8325);
newPoint.setY(-7.13438);
newPoint.setVel(0.98374);
path.push_back(newPoint);
newPoint.setX(15.7015);
newPoint.setY(-6.40493);
newPoint.setVel(0.98078);
path.push_back(newPoint);
newPoint.setX(15.5822);
newPoint.setY(-5.67568);
newPoint.setVel(0.977927);
path.push_back(newPoint);
newPoint.setX(15.4754);
newPoint.setY(-4.94664);
newPoint.setVel(0.975219);
path.push_back(newPoint);
newPoint.setX(15.3815);
newPoint.setY(-4.21781);
newPoint.setVel(0.972693);
path.push_back(newPoint);
newPoint.setX(15.301);
newPoint.setY(-3.48922);
newPoint.setVel(0.97039);
path.push_back(newPoint);
newPoint.setX(15.2345);
newPoint.setY(-2.76087);
newPoint.setVel(0.968352);
path.push_back(newPoint);
newPoint.setX(15.1826);
newPoint.setY(-2.03276);
newPoint.setVel(0.966624);
path.push_back(newPoint);
newPoint.setX(15.1457);
newPoint.setY(-1.3049);
newPoint.setVel(0.965251);
path.push_back(newPoint);
newPoint.setX(15.1245);
newPoint.setY(-0.577314);
newPoint.setVel(0.964281);
path.push_back(newPoint);
newPoint.setX(15.1195);
newPoint.setY(0.15);
newPoint.setVel(0.969722);
path.push_back(newPoint);
newPoint.setX(15.1337);
newPoint.setY(0.901226);
newPoint.setVel(0.990598);
path.push_back(newPoint);
newPoint.setX(15.1689);
newPoint.setY(1.66751);
newPoint.setVel(1.01075);
path.push_back(newPoint);
newPoint.setX(15.2242);
newPoint.setY(2.44778);
newPoint.setVel(1.03005);
path.push_back(newPoint);
newPoint.setX(15.2989);
newPoint.setY(3.24093);
newPoint.setVel(1.04838);
path.push_back(newPoint);
newPoint.setX(15.392);
newPoint.setY(4.04588);
newPoint.setVel(1.06566);
path.push_back(newPoint);
newPoint.setX(15.5026);
newPoint.setY(4.86156);
newPoint.setVel(1.08181);
path.push_back(newPoint);
newPoint.setX(15.6298);
newPoint.setY(5.68686);
newPoint.setVel(1.09674);
path.push_back(newPoint);
newPoint.setX(15.7728);
newPoint.setY(6.5207);
newPoint.setVel(1.11041);
path.push_back(newPoint);
newPoint.setX(15.9307);
newPoint.setY(7.362);
newPoint.setVel(1.12275);
path.push_back(newPoint);
newPoint.setX(16.1027);
newPoint.setY(8.20967);
newPoint.setVel(1.13372);
path.push_back(newPoint);
newPoint.setX(16.2877);
newPoint.setY(9.06263);
newPoint.setVel(1.14327);
path.push_back(newPoint);
newPoint.setX(16.485);
newPoint.setY(9.91978);
newPoint.setVel(1.15138);
path.push_back(newPoint);
newPoint.setX(16.6937);
newPoint.setY(10.78);
newPoint.setVel(1.158);
path.push_back(newPoint);
newPoint.setX(16.9129);
newPoint.setY(11.6423);
newPoint.setVel(1.1631);
path.push_back(newPoint);
newPoint.setX(17.1418);
newPoint.setY(12.5056);
newPoint.setVel(1.16667);
path.push_back(newPoint);
newPoint.setX(17.3793);
newPoint.setY(13.3686);
newPoint.setVel(1.16867);
path.push_back(newPoint);
newPoint.setX(17.6247);
newPoint.setY(14.2305);
newPoint.setVel(1.16909);
path.push_back(newPoint);
newPoint.setX(17.8771);
newPoint.setY(15.09);
newPoint.setVel(1.16791);
path.push_back(newPoint);
newPoint.setX(18.1356);
newPoint.setY(15.9461);
newPoint.setVel(1.16511);
path.push_back(newPoint);
newPoint.setX(18.3994);
newPoint.setY(16.7977);
newPoint.setVel(1.16068);
path.push_back(newPoint);
newPoint.setX(18.6674);
newPoint.setY(17.6438);
newPoint.setVel(1.15461);
path.push_back(newPoint);
newPoint.setX(18.939);
newPoint.setY(18.4832);
newPoint.setVel(1.14687);
path.push_back(newPoint);
newPoint.setX(19.2131);
newPoint.setY(19.3148);
newPoint.setVel(1.13747);
path.push_back(newPoint);
newPoint.setX(19.489);
newPoint.setY(20.1376);
newPoint.setVel(1.12639);
path.push_back(newPoint);
newPoint.setX(19.7656);
newPoint.setY(20.9504);
newPoint.setVel(1.11363);
path.push_back(newPoint);
newPoint.setX(20.0422);
newPoint.setY(21.7523);
newPoint.setVel(1.09917);
path.push_back(newPoint);
newPoint.setX(20.3179);
newPoint.setY(22.542);
newPoint.setVel(1.08301);
path.push_back(newPoint);
newPoint.setX(20.5918);
newPoint.setY(23.3186);
newPoint.setVel(1.06514);
path.push_back(newPoint);
newPoint.setX(20.863);
newPoint.setY(24.0808);
newPoint.setVel(1.04557);
path.push_back(newPoint);
newPoint.setX(21.1306);
newPoint.setY(24.8278);
newPoint.setVel(1.02428);
path.push_back(newPoint);
newPoint.setX(21.3938);
newPoint.setY(25.5583);
newPoint.setVel(1.00127);
path.push_back(newPoint);
newPoint.setX(21.6517);
newPoint.setY(26.2712);
newPoint.setVel(0.976538);
path.push_back(newPoint);
newPoint.setX(21.9034);
newPoint.setY(26.9656);
newPoint.setVel(0.950086);
path.push_back(newPoint);
newPoint.setX(22.1479);
newPoint.setY(27.6402);
newPoint.setVel(0.92191);
path.push_back(newPoint);
newPoint.setX(22.3846);
newPoint.setY(28.294);
newPoint.setVel(0.892012);
path.push_back(newPoint);
newPoint.setX(22.6123);
newPoint.setY(28.926);
newPoint.setVel(0.860393);
path.push_back(newPoint);
newPoint.setX(22.8304);
newPoint.setY(29.535);
newPoint.setVel(0.827056);
path.push_back(newPoint);
newPoint.setX(23.0379);
newPoint.setY(30.12);
newPoint.setVel(0.792006);
path.push_back(newPoint);
newPoint.setX(23.2338);
newPoint.setY(30.6798);
newPoint.setVel(0.755252);
path.push_back(newPoint);
newPoint.setX(23.4175);
newPoint.setY(31.2134);
newPoint.setVel(0.716805);
path.push_back(newPoint);
newPoint.setX(23.5879);
newPoint.setY(31.7197);
newPoint.setVel(0.676679);
path.push_back(newPoint);
newPoint.setX(23.7442);
newPoint.setY(32.1976);
newPoint.setVel(0.553414);
path.push_back(newPoint);
newPoint.setX(23.8854);
newPoint.setY(32.646);
newPoint.setVel(0.330216);
path.push_back(newPoint);
newPoint.setX(24.0109);
newPoint.setY(33.0638);
newPoint.setVel(0.146167);
path.push_back(newPoint);
newPoint.setX(24.1195);
newPoint.setY(33.45);
newPoint.setVel(0);
path.push_back(newPoint);



    return path;

//     fileName = "./src/ROUTES/" + fileName;
//     ifstream file(fileName);
//    vector<Waypoint> path;

//    if (!file.is_open()) {
//        cout << "Unable to open file";
//        exit(1); // terminate with error
//    }

//    string line;
//    while (getline(file, line)) {
//        istringstream iss(line);
//        double x, y, vel;
//        char comma;

//        if (!(iss >> x >> comma >> y >> comma >> vel)) { break; } // error

//        std::cout << x << ' ' << y << ' ' << vel << '\n';

//        Waypoint newPoint = Waypoint(x, y);
//        newPoint.setVel(vel);
//        path.push_back(newPoint);
//    }

//    file.close();

//    return path;


    // fileName = "./ROUTES/" + fileName;
    // FILE *lol = freopen(fileName.c_str(), "r", stdin);

    // vector<Waypoint> path;

    // while(!cin.eof()) {
    //     double x, y, vel;
    //     cin >> x >> y >> vel;
        
    //     std::cout << x << ' ' << y << ' ' << vel << '\n';

    //     Waypoint newPoint = Waypoint(x, y);
    //     newPoint.setVel(vel);
    //     path.push_back(newPoint);
    // }
    
    // fclose(lol);

    // return path;

    
    // vector<Waypoint> points;
    // ifstream file(fileName);
    // string line;
    // while (getline(file, line)) {
    //     stringstream ss(line);
    //     string x, y, velocity;
    //     getline(ss, x, ',');
    //     getline(ss, y, ',');
    //     getline(ss, velocity);

    //     Waypoint newPoint = Waypoint(stof(x), stof(y));
    //     newPoint.setVel(stof(velocity));

    //     points.push_back(newPoint);
    // }
    // return points;
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

    // //Step 5a. Calculate Velocities
    // for(int i=0; i<pathToFollow.size(); i++) {
    //     if(pathToFollow[i].getCurv() == 0) {
    //         pathToFollow[i].setVel(maxVel);
    //     }
    //     else {
    //         pathToFollow[i].setVel(min(velocityK/(pathToFollow[i].getCurv()), maxVel)); 
    //     }
    // }

    // //Step 5b & c
    // pathToFollow[pathToFollow.size()-1].setVel(0);

    // for(int i=pathToFollow.size()-1; i>=0; i--) {
    //     float dist = distance(pathToFollow[i+1], pathToFollow[i]);
    //     float newVel = sqrt(pow(pathToFollow[i+1].getVel(), 2) + 2 * maxA * dist);

    //     pathToFollow[i].setVel(min(pathToFollow[i].getVel(), newVel));
    // }

    //Step 5 - Michael + Brandon COOK version:
    float pathMaxVelValue = -1;
    for(int i=0; i<pathToFollow.size(); i++) {
        pathMaxVelValue = max(pathToFollow[i].getVel(), pathMaxVelValue);
    }
    for(int i=0; i<pathToFollow.size(); i++) {
        pathToFollow[i].setVel(pathToFollow[i].getVel() / pathMaxVelValue * maxVel);
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
    return -1*(FR.get_actual_velocity()+MR.get_actual_velocity()+BR.get_actual_velocity())/3; //bcz all motors are false?!?!
}

float getLeftVel() {
    return (FL.get_actual_velocity()+ML.get_actual_velocity()+BL.get_actual_velocity())/3;
}