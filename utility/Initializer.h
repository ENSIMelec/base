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
#include "../strategy/Strategy.h"
#include "../../ResistanceReader.h"
#include <thread>

using namespace std;
const string RESOURCES_PATH = "../res/";

class Initializer {
public:
    static Configuration * start(bool log = true);
    static void end();

    static bool isLidarActivated() {return activateLidar;}
    // ----- Getters -----
    static Configuration * getConfiguration() {return configuration;}
    static Odometry * getOdometry() {return odometry;}
    static MotorManager * getMotorManager() {return motorManager;}
    static Controller * getController() {return controller;}
    static ActionManager * getActionManager() {return actionManager;}
    static SerialCoderManager * getSerialCoderManager() {return odometry->getSerialCoderManager();}
    static Strategy * getStrategy() {return strategy;}

    // ----- Setters -----
    static void setStrategy(Strategy * s) {strategy = s;}

    static void startLidar();
    static void setLidarActivated(bool activated);

    static int getColor();
    static void setColor(int color);
private:
    inline static MotorManager *motorManager = nullptr;
    inline static Odometry *odometry = nullptr;
    inline static Controller *controller = nullptr;
    inline static ActionManager *actionManager = nullptr;
    inline static Strategy *strategy = nullptr;

    inline static thread * lidarThread;
    inline static int activateLidar;

    inline static int motor_fd;
    inline static int servos_fd;

    static int initWiringPi();
    static void initMotorManager();
    static void initController();
    static void initOdometry();
    static void initActionManager();
    static void initLidar();

protected:
    inline static bool allowLogging;
    inline static Configuration *configuration = nullptr;
    inline static int color = YELLOW;
};

#endif //KRABBS_INITIALIZE_H
