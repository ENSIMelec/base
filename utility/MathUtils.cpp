//
// Created by Taoufik on 16/11/2019.
//

#include "MathUtils.h"
#include "iostream"

using namespace std;

float MathUtils::inrange(float x, float min, float max) {
    return periodicmod(x - min, max - min) + min;
}

double MathUtils::constrain(double value, double min, double max)
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}

float MathUtils::periodicmod(float x, float y) {
    return fmod(fmod(x, y) + y, y);
}

float MathUtils::deg2rad(double deg) {
    return deg * (M_PI / 180);
}
float MathUtils::rad2deg(double rad) {
    return rad * (180 / M_PI);
}
float MathUtils::micros2sec(double sec) {
    return sec * 0.000001;
}
float MathUtils::millis2sec(double millis) {
    return millis * 0.001;
}

/**
 * Return an angle value between -180 and 180
 * @param angle Between -infinity and +infinity
 * @return
 */
double MathUtils::normalizeAngle(double angle, bool inRad) {
    double base = (inRad) ? M_PI : 180;

    int q = (int) (abs(angle) / base);
    double r = fmod(abs(angle), base);

    double sign = (angle < 0) ? -1 : 1;

    double normalizedAngle = 0;
    if((q % 2) == 0) {
        normalizedAngle = sign * r;
    } else {
        normalizedAngle = -sign * (base - r);
    }

//    cout << "[MathUtils] Angle : " << angle << "\tNormalized Angle : " << normalizedAngle << endl;
    return normalizedAngle;
}
