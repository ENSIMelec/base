//
// Created by Tom on 05/05/2022.
//

#include "../utility/MathUtils.h"
#include "TranslationController.h"

void TranslationController::update(double x, double y, double theta) {

    currentPosition.x = x;
    currentPosition.y = y;
    currentPosition.theta = theta;

    double distance = calculateDistanceError() * Pk_distance;
    double distanceCommand = MathUtils::constrain(distance, 0.0, 100.0);

    double angleError = calculateAngleError();
    double angleCommand = angleError * Pk_angle;

    // Apply commands
    leftCommand = (int) (distanceCommand - angleCommand);
    rightCommand = (int) (distanceCommand + angleCommand);

    cout << "TRANSLATION:: " << "distanceCommand: " << distanceCommand << endl;
    cout << "TRANSLATION:: " << "angleCommand : " << angleCommand << endl;
    cout << "TRANSLATION:: " << "leftCommand : " << leftCommand << "\trightCommand : " << rightCommand << endl;
}

void TranslationController::setTargetXY(double x, double y) {
    targetPosition.x = x;
    targetPosition.y = y;
}

bool TranslationController::isTargetReached() const {
    // Get the distance between actual location and target
    double distance = calculateDistanceError();
    return distance < DISTANCE_THRESHOLD;
}

double TranslationController::calculateAngleError() const {

    // Calculate the angle error
    double dX = targetPosition.x - currentPosition.x;
    double dY = targetPosition.y - currentPosition.y;

    double targetAngle = atan(dY / dX);

    cout << "[ANGLE CALCULATION] dx : " << dX << " dy : " << dY << endl;
    cout << "[ANGLE CALCULATION] target angle : " << targetAngle << endl;
    cout << "[ANGLE CALCULATION] distance : " << sqrt(dX * dX + dY * dY) << endl;
    if(targetAngle < - M_PI) {
        targetAngle += M_2_PI;
    } else if(targetAngle > M_PI) {
        targetAngle -= M_2_PI;
    }

    return targetAngle - currentPosition.theta;
}

/**
 * Get the distance between the current location and target
 */
double TranslationController::calculateDistanceError() const {
    double dX = currentPosition.x - targetPosition.x;
    double dY = currentPosition.y - targetPosition.y;

    return sqrt(dX * dX + dY * dY);
}
