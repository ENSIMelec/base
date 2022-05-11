//
// Created by Taoufik on 16/11/2019.
//

#ifndef ROBOT_MATHUTILS_H
#define ROBOT_MATHUTILS_H

#include "cmath"


class MathUtils final {

public:

    static float periodicmod(float x, float y);
    static float inrange(float x, float min, float max);
    static double constrain(double value, double min, double max);
    static float deg2rad(double deg);
    static float rad2deg(double rad);
    static float micros2sec(double sec);
	static float millis2sec(double millis);

    static double normalAngleRange(double angle, bool rad = true);
};


#endif //ROBOT_MATHUTILS_H
