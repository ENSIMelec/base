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

// Setup basic IO like the LED, the Emergency stop button and the Jack switch
void Utils::initGPIO(){
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_ARU, INPUT);
    pinMode(PIN_JACK, INPUT);
    pullUpDnControl(PIN_JACK, PUD_UP);
}

// Toggle the status LED on top of the robot
void Utils::toggleLED(bool state){
    digitalWrite(PIN_LED, state);
}

// Check if the ARU is pressed
bool Utils::isARUPressed(){
    return !digitalRead(PIN_ARU);
}

// Check if the Jack is removed
bool Utils::isJackRemoved(){
    return digitalRead(PIN_JACK);
}