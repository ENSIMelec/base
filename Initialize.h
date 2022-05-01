//
// Created by Tom on 30/04/2022.
//

#ifndef KRABBS_INITIALIZE_H
#define KRABBS_INITIALIZE_H

#include <string>
#include "Config.h"
#include "MotorManager.h"
#include "Controller.h"
#include "ActionManager.h"

using namespace std;
const string RESOURCES_PATH = "/home/pi/Documents/Krabbs/res/";

class Initialize {
public:
    static Config * start(bool log = true);
    static Config * getConfiguration() {return configuration;}

private:
    inline static bool allowLogging;

    inline static Config *configuration;
    inline static MotorManager *motorManager;
    inline static Odometry *odometry;
    inline static Controller *controller;
    inline static ActionManager *actionManager;

    inline static int motor_fd;
    inline static int servos_fd;

    static int initWiringPi();
    static void initMotorManager();
    static void initController();

    static void initOdometry();

    static void initActionManager();

};

#endif //KRABBS_INITIALIZE_H
