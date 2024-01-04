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
constexpr float kV = 1/maximumVelocity; //VIVEK NEED TO TUNE!!
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

        std::cout << leftVel << " " << rightVel << "\n";

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
    
    LeftDT.set_brake_modes(E_MOTOR_BRAKE_HOLD);
    RightDT.set_brake_modes(E_MOTOR_BRAKE_HOLD);

    stopMotors();

}

vector<Waypoint> readPoints(string fileName) {

    vector<Waypoint> path;
Waypoint newPoint = Waypoint(0, 0, 0);
newPoint.setX(26.2195);
newPoint.setY(-38.55);
newPoint.setVel(1);
path.push_back(newPoint);
newPoint.setX(26.2311);
newPoint.setY(-37.795);
newPoint.setVel(1.00948);
path.push_back(newPoint);
newPoint.setX(26.2428);
newPoint.setY(-37.0329);
newPoint.setVel(1.01868);
path.push_back(newPoint);
newPoint.setX(26.2546);
newPoint.setY(-36.2641);
newPoint.setVel(1.0276);
path.push_back(newPoint);
newPoint.setX(26.2665);
newPoint.setY(-35.4887);
newPoint.setVel(1.03624);
path.push_back(newPoint);
newPoint.setX(26.2785);
newPoint.setY(-34.7068);
newPoint.setVel(1.04461);
path.push_back(newPoint);
newPoint.setX(26.2906);
newPoint.setY(-33.9188);
newPoint.setVel(1.0527);
path.push_back(newPoint);
newPoint.setX(26.3028);
newPoint.setY(-33.1248);
newPoint.setVel(1.06051);
path.push_back(newPoint);
newPoint.setX(26.3151);
newPoint.setY(-32.325);
newPoint.setVel(1.06805);
path.push_back(newPoint);
newPoint.setX(26.3275);
newPoint.setY(-31.5197);
newPoint.setVel(1.07531);
path.push_back(newPoint);
newPoint.setX(26.3399);
newPoint.setY(-30.709);
newPoint.setVel(1.08229);
path.push_back(newPoint);
newPoint.setX(26.3525);
newPoint.setY(-29.8932);
newPoint.setVel(1.08899);
path.push_back(newPoint);
newPoint.setX(26.3651);
newPoint.setY(-29.0725);
newPoint.setVel(1.09541);
path.push_back(newPoint);
newPoint.setX(26.3777);
newPoint.setY(-28.247);
newPoint.setVel(1.10156);
path.push_back(newPoint);
newPoint.setX(26.3905);
newPoint.setY(-27.417);
newPoint.setVel(1.10743);
path.push_back(newPoint);
newPoint.setX(26.4033);
newPoint.setY(-26.5827);
newPoint.setVel(1.11303);
path.push_back(newPoint);
newPoint.setX(26.4162);
newPoint.setY(-25.7444);
newPoint.setVel(1.11834);
path.push_back(newPoint);
newPoint.setX(26.4291);
newPoint.setY(-24.9021);
newPoint.setVel(1.12338);
path.push_back(newPoint);
newPoint.setX(26.4421);
newPoint.setY(-24.0561);
newPoint.setVel(1.12814);
path.push_back(newPoint);
newPoint.setX(26.4551);
newPoint.setY(-23.2067);
newPoint.setVel(1.13263);
path.push_back(newPoint);
newPoint.setX(26.4682);
newPoint.setY(-22.354);
newPoint.setVel(1.13683);
path.push_back(newPoint);
newPoint.setX(26.4814);
newPoint.setY(-21.4982);
newPoint.setVel(1.14076);
path.push_back(newPoint);
newPoint.setX(26.4945);
newPoint.setY(-20.6396);
newPoint.setVel(1.14442);
path.push_back(newPoint);
newPoint.setX(26.5078);
newPoint.setY(-19.7784);
newPoint.setVel(1.14779);
path.push_back(newPoint);
newPoint.setX(26.521);
newPoint.setY(-18.9147);
newPoint.setVel(1.15089);
path.push_back(newPoint);
newPoint.setX(26.5343);
newPoint.setY(-18.0488);
newPoint.setVel(1.15371);
path.push_back(newPoint);
newPoint.setX(26.5477);
newPoint.setY(-17.1809);
newPoint.setVel(1.15625);
path.push_back(newPoint);
newPoint.setX(26.561);
newPoint.setY(-16.3111);
newPoint.setVel(1.15852);
path.push_back(newPoint);
newPoint.setX(26.5744);
newPoint.setY(-15.4398);
newPoint.setVel(1.1605);
path.push_back(newPoint);
newPoint.setX(26.5878);
newPoint.setY(-14.5671);
newPoint.setVel(1.16221);
path.push_back(newPoint);
newPoint.setX(26.6012);
newPoint.setY(-13.6932);
newPoint.setVel(1.16365);
path.push_back(newPoint);
newPoint.setX(26.6146);
newPoint.setY(-12.8183);
newPoint.setVel(1.1648);
path.push_back(newPoint);
newPoint.setX(26.6281);
newPoint.setY(-11.9427);
newPoint.setVel(1.16568);
path.push_back(newPoint);
newPoint.setX(26.6415);
newPoint.setY(-11.0665);
newPoint.setVel(1.16628);
path.push_back(newPoint);
newPoint.setX(26.655);
newPoint.setY(-10.19);
newPoint.setVel(1.1666);
path.push_back(newPoint);
newPoint.setX(26.6685);
newPoint.setY(-9.31333);
newPoint.setVel(1.16665);
path.push_back(newPoint);
newPoint.setX(26.6819);
newPoint.setY(-8.43673);
newPoint.setVel(1.16642);
path.push_back(newPoint);
newPoint.setX(26.6954);
newPoint.setY(-7.5604);
newPoint.setVel(1.16591);
path.push_back(newPoint);
newPoint.setX(26.7088);
newPoint.setY(-6.68457);
newPoint.setVel(1.16513);
path.push_back(newPoint);
newPoint.setX(26.7223);
newPoint.setY(-5.80942);
newPoint.setVel(1.16406);
path.push_back(newPoint);
newPoint.setX(26.7357);
newPoint.setY(-4.93518);
newPoint.setVel(1.16272);
path.push_back(newPoint);
newPoint.setX(26.7491);
newPoint.setY(-4.06206);
newPoint.setVel(1.1611);
path.push_back(newPoint);
newPoint.setX(26.7625);
newPoint.setY(-3.19025);
newPoint.setVel(1.15921);
path.push_back(newPoint);
newPoint.setX(26.7758);
newPoint.setY(-2.31997);
newPoint.setVel(1.15704);
path.push_back(newPoint);
newPoint.setX(26.7892);
newPoint.setY(-1.45143);
newPoint.setVel(1.15459);
path.push_back(newPoint);
newPoint.setX(26.8025);
newPoint.setY(-0.584829);
newPoint.setVel(1.15186);
path.push_back(newPoint);
newPoint.setX(26.8158);
newPoint.setY(0.279614);
newPoint.setVel(1.14885);
path.push_back(newPoint);
newPoint.setX(26.829);
newPoint.setY(1.1417);
newPoint.setVel(1.14557);
path.push_back(newPoint);
newPoint.setX(26.8422);
newPoint.setY(2.00121);
newPoint.setVel(1.14201);
path.push_back(newPoint);
newPoint.setX(26.8554);
newPoint.setY(2.85794);
newPoint.setVel(1.13817);
path.push_back(newPoint);
newPoint.setX(26.8685);
newPoint.setY(3.71168);
newPoint.setVel(1.13406);
path.push_back(newPoint);
newPoint.setX(26.8815);
newPoint.setY(4.56223);
newPoint.setVel(1.12967);
path.push_back(newPoint);
newPoint.setX(26.8945);
newPoint.setY(5.40938);
newPoint.setVel(1.125);
path.push_back(newPoint);
newPoint.setX(26.9075);
newPoint.setY(6.25291);
newPoint.setVel(1.12005);
path.push_back(newPoint);
newPoint.setX(26.9204);
newPoint.setY(7.09262);
newPoint.setVel(1.11483);
path.push_back(newPoint);
newPoint.setX(26.9332);
newPoint.setY(7.9283);
newPoint.setVel(1.10933);
path.push_back(newPoint);
newPoint.setX(26.946);
newPoint.setY(8.75974);
newPoint.setVel(1.10355);
path.push_back(newPoint);
newPoint.setX(26.9587);
newPoint.setY(9.58674);
newPoint.setVel(1.09749);
path.push_back(newPoint);
newPoint.setX(26.9713);
newPoint.setY(10.4091);
newPoint.setVel(1.09116);
path.push_back(newPoint);
newPoint.setX(26.9839);
newPoint.setY(11.2266);
newPoint.setVel(1.08455);
path.push_back(newPoint);
newPoint.setX(26.9963);
newPoint.setY(12.039);
newPoint.setVel(1.07766);
path.push_back(newPoint);
newPoint.setX(27.0087);
newPoint.setY(12.8461);
newPoint.setVel(1.0705);
path.push_back(newPoint);
newPoint.setX(27.021);
newPoint.setY(13.6477);
newPoint.setVel(1.06305);
path.push_back(newPoint);
newPoint.setX(27.0333);
newPoint.setY(14.4437);
newPoint.setVel(1.05533);
path.push_back(newPoint);
newPoint.setX(27.0454);
newPoint.setY(15.2337);
newPoint.setVel(1.04734);
path.push_back(newPoint);
newPoint.setX(27.0574);
newPoint.setY(16.0176);
newPoint.setVel(1.03906);
path.push_back(newPoint);
newPoint.setX(27.0694);
newPoint.setY(16.7952);
newPoint.setVel(1.03051);
path.push_back(newPoint);
newPoint.setX(27.0812);
newPoint.setY(17.5663);
newPoint.setVel(1.02168);
path.push_back(newPoint);
newPoint.setX(27.0929);
newPoint.setY(18.3306);
newPoint.setVel(1.01257);
path.push_back(newPoint);
newPoint.setX(27.1046);
newPoint.setY(19.088);
newPoint.setVel(1.00319);
path.push_back(newPoint);
newPoint.setX(27.1161);
newPoint.setY(19.8382);
newPoint.setVel(0.993528);
path.push_back(newPoint);
newPoint.setX(27.1275);
newPoint.setY(20.5811);
newPoint.setVel(0.983589);
path.push_back(newPoint);
newPoint.setX(27.1388);
newPoint.setY(21.3164);
newPoint.setVel(0.973373);
path.push_back(newPoint);
newPoint.setX(27.15);
newPoint.setY(22.0439);
newPoint.setVel(0.962879);
path.push_back(newPoint);
newPoint.setX(27.161);
newPoint.setY(22.7634);
newPoint.setVel(0.952108);
path.push_back(newPoint);
newPoint.setX(27.1719);
newPoint.setY(23.4747);
newPoint.setVel(0.94106);
path.push_back(newPoint);
newPoint.setX(27.1827);
newPoint.setY(24.1776);
newPoint.setVel(0.929734);
path.push_back(newPoint);
newPoint.setX(27.1934);
newPoint.setY(24.8719);
newPoint.setVel(0.918131);
path.push_back(newPoint);
newPoint.setX(27.2039);
newPoint.setY(25.5574);
newPoint.setVel(0.90625);
path.push_back(newPoint);
newPoint.setX(27.2143);
newPoint.setY(26.2339);
newPoint.setVel(0.894092);
path.push_back(newPoint);
newPoint.setX(27.2245);
newPoint.setY(26.9011);
newPoint.setVel(0.881657);
path.push_back(newPoint);
newPoint.setX(27.2346);
newPoint.setY(27.5588);
newPoint.setVel(0.868944);
path.push_back(newPoint);
newPoint.setX(27.2446);
newPoint.setY(28.2069);
newPoint.setVel(0.855954);
path.push_back(newPoint);
newPoint.setX(27.2544);
newPoint.setY(28.8452);
newPoint.setVel(0.842687);
path.push_back(newPoint);
newPoint.setX(27.264);
newPoint.setY(29.4733);
newPoint.setVel(0.829142);
path.push_back(newPoint);
newPoint.setX(27.2735);
newPoint.setY(30.0912);
newPoint.setVel(0.81532);
path.push_back(newPoint);
newPoint.setX(27.2828);
newPoint.setY(30.6986);
newPoint.setVel(0.80122);
path.push_back(newPoint);
newPoint.setX(27.292);
newPoint.setY(31.2953);
newPoint.setVel(0.786844);
path.push_back(newPoint);
newPoint.setX(27.301);
newPoint.setY(31.881);
newPoint.setVel(0.772189);
path.push_back(newPoint);
newPoint.setX(27.3098);
newPoint.setY(32.4557);
newPoint.setVel(0.757258);
path.push_back(newPoint);
newPoint.setX(27.3185);
newPoint.setY(33.019);
newPoint.setVel(0.742049);
path.push_back(newPoint);
newPoint.setX(27.327);
newPoint.setY(33.5709);
newPoint.setVel(0.726562);
path.push_back(newPoint);
newPoint.setX(27.3352);
newPoint.setY(34.1109);
newPoint.setVel(0.710799);
path.push_back(newPoint);
newPoint.setX(27.3434);
newPoint.setY(34.639);
newPoint.setVel(0.694758);
path.push_back(newPoint);
newPoint.setX(27.3513);
newPoint.setY(35.155);
newPoint.setVel(0.678439);
path.push_back(newPoint);
newPoint.setX(27.359);
newPoint.setY(35.6586);
newPoint.setVel(0.661844);
path.push_back(newPoint);
newPoint.setX(27.3666);
newPoint.setY(36.1496);
newPoint.setVel(0.64497);
path.push_back(newPoint);
newPoint.setX(27.3739);
newPoint.setY(36.6278);
newPoint.setVel(0.62782);
path.push_back(newPoint);
newPoint.setX(27.381);
newPoint.setY(37.0931);
newPoint.setVel(0.610392);
path.push_back(newPoint);
newPoint.setX(27.388);
newPoint.setY(37.5451);
newPoint.setVel(0.592687);
path.push_back(newPoint);
newPoint.setX(27.3947);
newPoint.setY(37.9837);
newPoint.setVel(0.574704);
path.push_back(newPoint);
newPoint.setX(27.4012);
newPoint.setY(38.4087);
newPoint.setVel(0.441963);
path.push_back(newPoint);
newPoint.setX(27.4076);
newPoint.setY(38.8199);
newPoint.setVel(0.279768);
path.push_back(newPoint);
newPoint.setX(27.4137);
newPoint.setY(39.2171);
newPoint.setVel(0.132527);
path.push_back(newPoint);
newPoint.setX(27.4195);
newPoint.setY(39.6);
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