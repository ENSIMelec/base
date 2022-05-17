//
// Created by Tom on 30/04/2022.
//
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "Initializer.h"
#include "../Lidar.h"
#include "../../ResistanceReader.h"
#include "../ui/UI.h"

#define EXIT_FAIL_I2C 1

Configuration * Initializer::start(bool log) {
    allowLogging = log;

    configuration = new Configuration(RESOURCES_PATH + "config.info");
    activateLidar = configuration->getInt("global.activate_lidar");

    initWiringPi();
    initOdometry();
    if(activateLidar == 1) initLidar();
    initMotorManager();
    initController();
    initActionManager();
    UI::init();

    return configuration;
}

int Initializer::initWiringPi() {
    if(allowLogging) cout << "Initializing WiringPi ... ";

    // The two init functions cannot be called at the same time
//    wiringPiSetupGpio();
    wiringPiSetup();

    // Arduino for the motors
    motor_fd = wiringPiI2CSetup(configuration->getInt("i2c.motors"));

    // Arduino for the servos
    servos_fd = wiringPiI2CSetup(configuration->getInt("i2c.servos"));

    // If not initialized, the addresses are negative
    if(motor_fd < 0 || servos_fd < 0)
        return EXIT_FAIL_I2C;

    if(allowLogging) cout << "done" << endl;
    return EXIT_SUCCESS;
}

void Initializer::initLidar() {
    Lidar::init();

    if(!Lidar::test()){
        cout << "Error while testing the lidar" << endl;
        exit(-1);
    }

    // Starting the thread
    int matchTime = configuration->getInt("global.match_time");
    lidarThread = new thread(Lidar::run, matchTime, 800);
}

void Initializer::initMotorManager() {
    if(allowLogging) cout << "Initializing the motor manager ... ";
	motorManager = new MotorManager(motor_fd);
    if(allowLogging) cout << "done" << endl;
}

void Initializer::initOdometry() {
    if(allowLogging) cout << "Initializing the odometry ... ";
    odometry = new Odometry(configuration);
    if(allowLogging) cout << "done" << endl;
}

void Initializer::initController() {
    if(allowLogging) cout << "Initializing the controller ... ";
    controller = new Controller(motorManager, odometry, configuration);
    if(allowLogging) cout << "done" << endl;
}

void Initializer::initActionManager() {
    if(allowLogging) cout << "Initializing the action manager ... ";
    int nbAx12 = configuration->getInt("global.nb_AX12");

    actionManager = new ActionManager(servos_fd, nbAx12, RESOURCES_PATH + "actions/");

    // Initialize the motors in the startup location
    if(nbAx12 > 0) actionManager->action("initialize.as");

    if(allowLogging) cout << "done" << endl;
}

void Initializer::end() {
    endwin();

    if(allowLogging) cout << "Quitting the application ... ";
    if(controller != nullptr) controller->stopMotors();

    // Resetting the AX12, very important if you want to start them afterward
    if(actionManager != nullptr) actionManager->close();

    // Closing file descriptors
    close(motor_fd);
    close(servos_fd);

    if(activateLidar == 1) {
        Lidar::stop();
        lidarThread->join();
    }

    if(allowLogging) cout << "done" << endl;
}


