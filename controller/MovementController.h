//
// Created by Tom on 08/05/2022.
//

#ifndef KRABBS_MOVEMENTCONTROLLER_H
#define KRABBS_MOVEMENTCONTROLLER_H

#include "../odometry/Odometry.h"

class MovementController {

public:
    MovementController(Configuration * configuration);

    void calculateCommands(double x, double y, double theta);
    void setTargetPosition(double x, double y);

    void calculateAngleCommand(double dX, double dY, double theta);
    void calculateDistanceCommand(double dX, double dY);

    double getDistanceCommand() const {return distanceCommand;}
    double getAngleCommand() const {return angleCommand;}

    bool isTargetReached() const {return targetReached;};

private:
    double distanceCommand = 0;
    double angleCommand = 0;

    double accelerationFactor = 0;
    double acceleration = 0;

    double Pk_distance = 0;
    double Pk_angle = 0;

    Location targetLocation;

    double distanceThreshold = 0;
    bool targetReached = false;
};


#endif //KRABBS_MOVEMENTCONTROLLER_H
