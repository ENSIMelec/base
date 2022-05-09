//
// Created by Taoufik on 16/11/2019.
//

#include "MathUtils.h"

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