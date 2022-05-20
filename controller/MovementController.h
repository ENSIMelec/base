//
// Created by Tom on 08/05/2022.
//

#ifndef KRABBS_MOVEMENTCONTROLLER_H
#define KRABBS_MOVEMENTCONTROLLER_H

#include "../odometry/Odometry.h"
#include "PID.h"
#include "../utility/Clock.h"

class MovementController {
public:
    enum Direction {
        FORWARD,
        BACKWARD
    };

    explicit MovementController(Configuration * configuration);

    void calculateCommands(double x, double y, double theta);
    void setTargetPosition(double x, double y);

    void calculateAngleCommand(double dX, double dY, double theta);
    void calculateDistanceCommand(double dX, double dY);

    [[nodiscard]] double getDistanceCommand() const {return distanceCommand;}
    [[nodiscard]] double getAngleCommand() const {return angleCommand;}
    [[nodiscard]] double getDistanceError() const {return distanceError;}
    [[nodiscard]] double getAngleError() const {return angleError;}
    [[nodiscard]] bool isTargetReached() const {return targetReached;};

    void setMaxSpeed(double max);
    void setTimeout(int t) {timeout = t;}
    void setDirection(Direction d) {direction = d;}
private:

    // Direction
    Direction direction = FORWARD;

    // Bypass the targetReached
    int timeout = 0;
    Clock timer;

    double distanceCommand = 0;
    double angleCommand = 0;

    double angleError = 0;
    double distanceError = 0;

    double accelerationFactor = 0;
    double acceleration = 0;

    Location targetLocation;

    double distanceThreshold = 0;
    bool targetReached = false;

    static double calculateDestinationAngle(double dX, double dY);

    PID * pid_distance;
    PID * pid_angle;

    void resetMaxSpeed();
    double originalMaxSpeed;
};


#endif //KRABBS_MOVEMENTCONTROLLER_H
