// #include "profile.h"

// using namespace pros;

// profile::profile(float v, float a) {
//     this->maxVel = v;
//     this->maxAccel = a;
//     this->dist = 0;
// }

// void profile::generate(float d) {
//     this->dist = d;
//     //Simple 1D Trap Profile can be broken into three parts: accelerate, cruise, deccelerate
//     //I'll use t1 t2 and t3 for this

//     //t1 = Accelerate from rest to max velocity {v = u + at}
//     /*
//         v = u + at
//             u = 0 (initial velocity)
//             t = t1 (total time for first segment)
//         v = at
//         t1 = v/a
//     */
//     float t1 = this->maxVel / this->maxAccel;

//     //t2 = Cruise at max velocity
//     /*
//         s = ut + 0.5at^2
//         d = vt + 0.5 * a * (t1)^2
//         vt = d - 0.5 * a * (t1)^2
//         t2 = (d - 0.5 * a * (t1)^2) / v
//     */
//     float t2 = (this->dist - 0.5 * this->maxAccel * t1 * t1) / this->maxVel;

//     //t3 = Deccelerate from max velocity to rest
//     //idk how to do this ill come back to this later
// }