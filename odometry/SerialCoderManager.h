#ifndef SERIALCODEURMANAGER_H_INCLUDED
#define SERIALCODEURMANAGER_H_INCLUDED

#include <cmath>
#include <chrono>
#include <cstdint>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define COMMAND_GET     0xFF
#define COMMAND_RESET   0xF0

#define START_BYTE      0xA5
#define STOP_BYTE       0x5A

#define TIMEOUT         100

#define CMD_RESET -3

class SerialCoderManager
{
public:

    int getRightTicks() const;
    int getLeftTicks() const;
    int getTime() const;

	SerialCoderManager();

	void readAndReset();
	void reset() const;

private:
    int encoders_fd;
    int leftTicks = 0, rightTicks = 0, elapsedTime = 0;
    int oldLeftTicks = 0, oldRightTicks = 0, oldElapsedTime = 0;

    static void printBuffer(int *buffer) ;
};

#endif //SERIALCODEURMANAGER_H_INCLUDED