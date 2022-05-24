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

    UI::init();
    UI::logAndRefresh("Started initialization :");

    initWiringPi();
    initOdometry();
    if(activateLidar == 1) initLidar();
    initMotorManager();
    initController();
    initActionManager();

    return configuration;
}

int Initializer::initWiringPi() {
    // The two init functions cannot be called at the same time
    wiringPiSetup();

    // Arduino for the motors
    motor_fd = wiringPiI2CSetup(configuration->getInt("i2c.motors"));

    // Arduino for the servos
    servos_fd = wiringPiI2CSetup(configuration->getInt("i2c.servos"));

    // If not initialized, the addresses are negative
    if(motor_fd < 0 || servos_fd < 0)
        return EXIT_FAIL_I2C;

    UI::logAndRefresh(" -- Wiring Pi done");
    return EXIT_SUCCESS;
}

void Initializer::initLidar() {
    Lidar::init();

//    if(!Lidar::test()){
//        UI::logAndRefresh("[LIDAR] Error while testing the lidar");
//        exit(-1);
//    }

    UI::logAndRefresh(" -- Lidar done");
}

void Initializer::startLidar() {
    // Starting the thread
    int matchTime = configuration->getInt("global.match_time");
    lidarThread = new thread(Lidar::run, matchTime, 800);
}

void Initializer::initMotorManager() {
	motorManager = new MotorManager(motor_fd);
    UI::logAndRefresh(" -- Motor Manager done");
}

void Initializer::initOdometry() {
    odometry = new Odometry(configuration);
    UI::logAndRefresh(" -- Odometry done");
}

void Initializer::initController() {
    controller = new Controller(motorManager, odometry, configuration);
    UI::logAndRefresh(" -- Controller done");
}

void Initializer::initActionManager() {
    int nbAx12 = configuration->getInt("global.nb_AX12");

    actionManager = new ActionManager(servos_fd, nbAx12, RESOURCES_PATH + "actions/");

    // Initialize the motors in the startup location
    if(nbAx12 > 0) actionManager->action("initialize.as");

    UI::logAndRefresh(" -- Action Manager done");
}

void Initializer::end() {
    UI::logAndRefresh("End of the program");
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
    UI::update();
    UI::end();
}

void Initializer::setLidarActivated(bool activated) {
    activateLidar = activated;
}


