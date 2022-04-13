//
// Created by Tom on 02/12/2021.
//
#include <thread>

#include "Utils.h"

void Utils::sleepMillis(int millis) {
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

double Utils::clamp(double value, double min, double max) {
    if(value <= min) {
        return min;
    } else if(value >= max) {
        return max;
    } else {
        return value;
    }
}