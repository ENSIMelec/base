//
// Created by Tom on 30/04/2022.
//
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "Initialize.h"

#define EXIT_FAIL_I2C 1

Config * Initialize::start(bool log) {
    allowLogging = log;

    configuration = new Config(RESOURCES_PATH + "config.info");

    initWiringPi();
    initMotorManager();
//    initOdometry();
    initController();
//    initActionManager();

    return configuration;
}

int Initialize::initWiringPi() {
    if(allowLogging) cout << "Initializing WiringPi ... ";
	wiringPiSetupGpio();

    // Arduino for the motors
    motor_fd = wiringPiI2CSetup(configuration->get_I2C_MOTEURS());

    // Arduino for the servos
    servos_fd = wiringPiI2CSetup(configuration->get_I2C_SERVOS());

    // If not initialized, the addresses are negative
    if(motor_fd < 0 || servos_fd < 0)
        return EXIT_FAIL_I2C;

    if(allowLogging) cout << "done" << endl;
    return EXIT_SUCCESS;
}

void Initialize::initMotorManager() {
    if(allowLogging) cout << "Initializing the motor manager ... ";
	motorManager = new MotorManager(motor_fd);
    if(allowLogging) cout << "done" << endl;
}

void Initialize::initOdometry() {
    if(allowLogging) cout << "Initializing the odometry ... ";
    odometry = new Odometry();
    if(allowLogging) cout << "done" << endl;
}

void Initialize::initController() {
    if(allowLogging) cout << "Initializing the controller ... ";
    controller = new Controller(odometry, motorManager, configuration);
    if(allowLogging) cout << "done" << endl;
}

void Initialize::initActionManager() {
    if(allowLogging) cout << "Initializing the action manager ... ";
    actionManager = new ActionManager(servos_fd, configuration->getNbAX12());
    if(allowLogging) cout << "done" << endl;
}

void Initialize::end() {
    if(allowLogging) cout << "Quitting the application ... ";
    controller->stopMotors();

    // Resetting the AX12, very important if you want to start them afterward
    actionManager->close();

    // Closing file descriptors
    close(motor_fd);
    close(servos_fd);

    if(allowLogging) cout << "done" << endl;
}

