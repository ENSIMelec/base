//
// Created by Tom on 05/05/2022.
//

#include "../utility/MathUtils.h"
#include "TranslationController.h"

void TranslationController::update(Position p_currentPosition) {

    currentPosition = p_currentPosition;

    double distance = calculateDistanceError() * Pk_distance;
    double distanceCommand = MathUtils::constrain(distance, 0.0, 100.0);

    double angleError = calculateAngleError();
    double angleCommand = angleError * Pk_angle;

    // Apply commands
    leftCommand = (int) (distanceCommand - angleCommand);
    rightCommand = (int) (distanceCommand + angleCommand);
}

void TranslationController::setTargetXY(double x, double y) {
    targetPosition.x = x;
    targetPosition.y = y;
}

bool TranslationController::isTargetReached() {
    // Get the distance between actual position and target
    double distance = calculateDistanceError();
    return distance < DISTANCE_THRESHOLD;
}

double TranslationController::calculateAngleError() {

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
 * Get the distance between the current position and target
 */
double TranslationController::calculateDistanceError() {
    double dX = currentPosition.x - targetPosition.x;
    double dY = currentPosition.y - targetPosition.y;

    return sqrt(dX * dX + dY * dY);
}
