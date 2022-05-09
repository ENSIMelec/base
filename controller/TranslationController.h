//
// Created by Tom on 05/05/2022.
//
#ifndef KRABBS_TRANSLATIONCONTROLLER_H
#define KRABBS_TRANSLATIONCONTROLLER_H

#include "../odometry/Odometry.h"

#define DISTANCE_THRESHOLD 5

class TranslationController {
public:
    TranslationController() = default;

    void update(double x, double y, double theta);

    int getLeftCommand() {return leftCommand;}
    int getRightCommand() {return rightCommand;}

    double calculateAngleError() const;
    double calculateDistanceError() const;

    bool isTargetReached() const;
    void setTargetXY(double x, double y);

private:
    int leftCommand;
    int rightCommand;

    // Correctors
    double Pk_angle = 200;
    double Pk_distance = 1;

    // Target location
    Location targetPosition;
    Location currentPosition;
};


#endif //KRABBS_TRANSLATIONCONTROLLER_H
