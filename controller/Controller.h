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
#include "AngleController.h"

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

    void stopMotors();
    bool isTargetReached();

    void debug();

    // ---------- Getters & Setters -----
    void setNextPoint(Point *point);
    Point * getCurrentPoint() {return currentPoint;}

    [[nodiscard]] double getDistCommand() const {return command.distance;}
    [[nodiscard]] double getAngleCommand() const {return command.angle;}
    [[nodiscard]] int getPWMLeft() const {return pwm.left;}
    [[nodiscard]] int getPWMRight() const {return pwm.right;}
    [[nodiscard]] double getDistanceError();
    [[nodiscard]] double getAngleError();
private:

    // Target location
    Location targetPosition;

    Odometry * odometry;
    MotorManager * motorManager;
    Configuration * config;

    double calculateAngleError();
    double calculateDistanceError();
    void calculateAngleCommands();
    void calculateDistanceCommands();
    void correctAngle();

    struct {
        double angle = 0;
        double distance = 0;
    } command;

    struct {
        int left = 0;
        int right = 0;
    } pwm;

    Point * currentPoint;

    bool angleIsCorrect = false;
    double targetAngle = 0;

    MovementController * movementController;
    AngleController * angleController;

    int activateMotors;
};


#endif //CONTROLLER_H
