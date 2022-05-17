//
// Created by Tom on 08/05/2022.
//

#ifndef KRABBS_MOVEMENTCONTROLLER_H
#define KRABBS_MOVEMENTCONTROLLER_H

#include "../odometry/Odometry.h"
#include "PID.h"

class MovementController {

public:
    MovementController(Configuration * configuration);

    void calculateCommands(double x, double y, double theta);
    void setTargetPosition(double x, double y);

    void calculateAngleCommand(double dX, double dY, double theta);
    void calculateDistanceCommand(double dX, double dY);

    [[nodiscard]] double getDistanceCommand() const {return distanceCommand;}
    [[nodiscard]] double getAngleCommand() const {return angleCommand;}
    [[nodiscard]] double getDistanceError() const {return distanceError;}
    [[nodiscard]] double getAngleError() const {return angleError;}

    bool isTargetReached() const {return targetReached;};

private:
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
};


#endif //KRABBS_MOVEMENTCONTROLLER_H
