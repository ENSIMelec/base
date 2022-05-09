//
// Created by Taoufik on 12/11/2019.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "PID.h"
#include "MotorManager.h"
#include "../utility/MathUtils.h"
#include "../utility/Configuration.h"
#include "../odometry/Odometry.h"
#include "../strategy/Point.h"
#include "MovementController.h"

class Controller {

#define DISTANCE_THRESHOLD 5    // Distance in millimeters

public:
    /**
     * Initialisation de l'asservisement
     * @param codeurs
     * @param motor
     * @param config
    */
    Controller(MotorManager * motorManager_, Odometry * odometry_, Configuration * config_);

    void update();

    void setTargetXY(int x, int y);
    void setTargetAngle(double angle);

    void stopMotors();
    bool isTargetReached();

    void setTargetPoint(Point* point);

    void debug();

    void setNextPoint(Point *point);

private:

    enum TrajectoryType {
        XY,
        ANGLE,
        NONE
    };
    TrajectoryType currentTrajectoryType = NONE;

    // Correctors
    double Pk_angle = 1;
    double Pk_distance = 1;

    // Target location
    Location targetPosition;

    Odometry * odometry;
    MotorManager * motorManager;
    Configuration * config;

    double calculateAngleError();
    double calculateDistanceError();

    struct {
        double angle = 0;
        double distance = 0;
    } command;

    struct {
        int left = 0;
        int right = 0;
    } pwm;

    Point * currentPoint;

    double accelerationFactor = 0;
    double accelerationCoeff;

    void calculateAngleCommands();

    bool angleIsCorrect = false;
    double targetAngle = 0;

    void calculateDistanceCommands();
    void correctAngle();

    MovementController * movementController;

};


#endif //CONTROLLER_H
