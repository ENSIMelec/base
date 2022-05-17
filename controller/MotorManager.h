#ifndef MOTOR_MANAGER_H_INCLUDED
#define MOTOR_MANAGER_H_INCLUDED

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <unistd.h>

/* The max speed must be between 0 and 255
 * Be careful when using values over 100
 */
 #define MAX_SPEED 200

class MotorManager
{

private:
    enum Direction {
        STOPPED = 0,
        FORWARD = 1,
        BACKWARD = 2
    };

    // Speed values are between 0 and 255, they should not be over 100 for the safety of the robot
    uint8_t leftSpeed = 0;
    uint8_t rightSpeed = 0;

    Direction leftDirection;
    Direction rightDirection;

    int i2c_fd;

    void capPWM(int *PWM);
    void sendData();

public:
     MotorManager(int i2c);

    void setOrder(int leftOrder, int rightOrder);

    void forward(int speed);
    void backward(int speed);
    void stop();

    static int getMaxAllowedSpeed() {
        return MAX_SPEED;
    }
};

#endif //MOTOR_MANAGER_H_INCLUDED