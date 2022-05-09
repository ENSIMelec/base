//
// Created by Tom on 30/04/2022.
//

#ifndef KRABBS_INITIALIZE_H
#define KRABBS_INITIALIZE_H

#include <string>
#include "../controller/MotorManager.h"
#include "../controller/Controller.h"
#include "../actuators/ActionManager.h"
#include "../odometry/Odometry.h"
#include "Configuration.h"
#include <thread>

using namespace std;
const string RESOURCES_PATH = "/home/pi/Documents/Krabbs/res/";

class Initializer {
public:
    static Configuration * start(bool log = true);
    static void end();

    // ----- Getters -----
    static Configuration * getConfiguration() {return configuration;}
    static Controller * getController() {return controller;}
    static Odometry * getOdometry() {return odometry;}
    static ActionManager * getActionManager() {return actionManager;}
    static MotorManager * getMotorManager() {return motorManager;}

private:
    inline static bool allowLogging;

    inline static Configuration *configuration = nullptr;
    inline static MotorManager *motorManager = nullptr;
    inline static Odometry *odometry = nullptr;
    inline static Controller *controller = nullptr;
    inline static ActionManager *actionManager = nullptr;

    inline static thread * lidarThread;

    inline static int motor_fd;
    inline static int servos_fd;

    static int initWiringPi();
    static void initMotorManager();
    static void initController();
    static void initOdometry();
    static void initActionManager();
    static void initLidar();
};

#endif //KRABBS_INITIALIZE_H
