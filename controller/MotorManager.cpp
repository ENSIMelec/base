#include "MotorManager.h"

// Uncomment to go in debug mod
//#define DEBUG_MOTOR_MANAGER

/**
 * The Arduino will receive 4 unsigned 8 bit integer values from the i2c bus.
 * The first two values are for the speed (PWM). The last two are for the direction of the motors.
 * There is 3 possible values :
 *  - 0 : STOPPED
 *  - 1 : FORWARD
 *  - 2 : BACKWARD
 *
 *  Note : the speed of the motors should not be over 100
 */

using namespace std;

/** @brief Constructor
 * @param i2c  The address on the i2c bus
 */
MotorManager::MotorManager(int i2c) {
    i2c_fd = i2c;
}

/**
 * @brief Load the order for the motors
 * The absolute value of each order affect the speed. The sign of the order changes the direction.
 * A null value means that the motor is stopped.
 */
void MotorManager::setOrder(int leftOrder, int rightOrder) {

    // We don't want values that are outside the range -MAX_SPEED to MAX_SPEED
    capPWM(&leftOrder);
    capPWM(&rightOrder);

    // The speed is a positive value between 0 and 255
    leftSpeed = (uint8_t) abs(leftOrder);
    rightSpeed = (uint8_t) abs(rightOrder);

    // The direction is defined by the sign
    if(leftOrder > 0) {
        leftDirection = FORWARD;
    } else if(leftOrder < 0) {
        leftDirection = BACKWARD;
    } else {
        leftDirection = STOPPED;
    }

    if(rightOrder > 0) {
        rightDirection = FORWARD;
    } else if(rightOrder < 0) {
        rightDirection = BACKWARD;
    } else {
        rightDirection = STOPPED;
    }

    // Send the data
    sendData();
}

/// @brief Shortcut to go forward
void MotorManager::forward(int speed) {
    setOrder(speed, speed);
	return;
}

/// @brief Shortcut to go backward
void MotorManager::backward(int speed) {
    setOrder(-speed, -speed);
	return;
}

/// @brief Cap the value between -255 and 255
void MotorManager::capPWM(int *PWM) {
	if(*PWM < -MAX_SPEED) *PWM = -MAX_SPEED;
	else if(*PWM > MAX_SPEED) *PWM = MAX_SPEED;
}

/// @brief Stop all the motors
void MotorManager::stop()
{
    leftSpeed = 0;
    leftDirection = STOPPED;

    rightSpeed = 0;
    rightDirection = STOPPED;

    sendData();
}

/// @brief Send the data using I2C
void MotorManager::sendData()
{
    // The bytes we will send
	uint8_t data[4];

	data[0] = (uint8_t) rightSpeed;
	data[1] = (uint8_t) leftSpeed;
	data[2] = (uint8_t) rightDirection;
	data[3] = (uint8_t) leftDirection;

#ifdef DEBUG_MOTOR_MANAGER
    cout << "[MOTOR DEBUG] Printing the content of data :" << endl;
    cout << "[MOTOR DEBUG] data[0] : " << unsigned(data[0]) << endl;
    cout << "[MOTOR DEBUG] data[1] : " << unsigned(data[1]) << endl;
    cout << "[MOTOR DEBUG] data[2] : " << unsigned(data[2]) << endl;
    cout << "[MOTOR DEBUG] data[3] : " << unsigned(data[3]) << endl;
#endif

	// Sending the byte array
	write(i2c_fd, data, 4);
}