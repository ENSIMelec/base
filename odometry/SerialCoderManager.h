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

#define CMD_RESET -3

class SerialCoderManager
{
public:

    int getRightTicks();
    int getLeftTicks();
    int getTime();

	SerialCoderManager();

	void readAndReset();
	void reset();

private:
    int encoders_fd;

    int leftTicks = 0, rightTicks = 0, tempsLast=0;
    int oldLeftTicks = 0, oldRightTicks = 0, oldTempsLast=0;
};

#endif //SERIALCODEURMANAGER_H_INCLUDED