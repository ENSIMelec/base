//
// Created by Taoufik on 12/11/2019.
//

#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H


#include "PID.h"
#include "SerialCodeurManager.h"
#include "Odometry.h"
#include "MotorManager.h"
#include "MathUtils.h"
#include "Config.h"
#include "QuadrampDerivate.h"
#include "Point.h"

class Controller {

#define DISTANCE_TRESHOLD 5

public:
    /**
     * Initialisation de l'asservisement
     * @param codeurs
     * @param motor
     * @param config
    */
    Controller(Odometry * odometry_, MotorManager * motorManager_, Config * config_) {
        this->odometry = odometry_;
        this->motorManager = motorManager_;
        this->config = config_;
    }

    void update();

    void setTargetXY(int x, int y);
    void setTargetAngle(double angle);

    void stopMotors();
    bool isTargetReached();
private:

    enum TrajectoryType {
        XY,
        ANGLE,
        NONE
    };
    TrajectoryType currentTrajectoryType = NONE;

    // Correctors
    double Pk_angle = 200;
    double Pk_distance = 1;

    // Target position
    Position targetPosition;

    //Odometry
    Odometry * odometry;

    // MoteurManager
    MotorManager * motorManager;

    //Config
    Config * config;

    double calculateAngleError();
    double calculateDistanceError();

    struct {
        double angle = 0;
        double distance = 0;
    } command;

    void absoluteAngle();
    void debug();
};


#endif //ROBOT_CONTROLLER_H
