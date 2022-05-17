//
// Created by Tom on 02/12/2021.
//

#ifndef KRABBS_UTILS_H
#define KRABBS_UTILS_H


#define PIN_LED 25
#define PIN_ARU 24
#define PIN_JACK 23

#include <wiringPi.h>


namespace Utils {
    /// @brief Function used to sleep a given amount of milliseconds
    void sleepMillis(int millis);

    /// @brief Clamp a value between a min and a max
    double clamp(double value, double min, double max);

    // Setup basic IO like the LED, the Emergency stop button and the Jack switch
    void initGPIO();

    // Toggle the status LED on top of the robot
    void toggleLED(bool state);

    // Check if the ARU is pressed
    bool isARUPressed();
    
    // Check if the Jack is removed
    bool isJackRemoved();
}
#endif //KRABBS_UTILS_H
