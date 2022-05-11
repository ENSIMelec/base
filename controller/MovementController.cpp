//
// Created by Tom on 08/05/2022.
//

#include <math.h>

#include "MovementController.h"
#include "../utility/MathUtils.h"

MovementController::MovementController(Configuration * configuration) {
    acceleration = configuration->getDouble("controller.movement.acceleration");
    Pk_distance = configuration->getDouble("controller.movement.Pk_distance");
    Pk_angle = configuration->getDouble("controller.movement.Pk_angle");
    distanceThreshold = configuration->getDouble("controller.movement.distance_threshold");
}

void MovementController::calculateCommands(double x, double y, double theta) {

    // Calculate the new acceleration
    if(accelerationFactor < 1) {
        accelerationFactor += acceleration;
    } else {
        accelerationFactor = 1;
    }

    // Calculate the commands
    double dX = targetLocation.x - x;
    double dY = targetLocation.y - y;

    calculateDistanceCommand(dX, dY);
    calculateAngleCommand(dX, dY, theta);

    // Check if the target is reached
    double distance = sqrt(dX * dX + dY * dY);
    if(distance < distanceThreshold) {
        targetReached = true;
    }
}

void MovementController::setTargetPosition(double x, double y) {
    targetLocation.x = x;
    targetLocation.y = y;

    accelerationFactor = 0;
}

void MovementController::calculateAngleCommand(double dX, double dY, double theta) {

    // Calculate the angle to the target
    if(dX == 0) {
        angleCommand = 0;
        return;
    }

    double angleError = atan(dY / dX);

    // Only when the angle should be over 90°
    if(dX < 0) {
        if(dY > 0) angleError += M_PI;
        if(dY < 0) angleError -= M_PI;
    }

    // Stay in the range -Pi, Pi
    if(angleError < - M_PI) {
        angleError += M_2_PI;
    } else if(angleError > M_PI) {
        angleError -= M_2_PI;
    }

    // Set the angle corresponding to the current angle
    angleError -= theta;

    cout << "[ANGLE CALCULATION] dx : " << dX << " dy : " << dY << endl;
    cout << "[ANGLE CALCULATION] Angle error : " << angleError << " (" << MathUtils::rad2deg(angleError) << "°)" << endl;

    // Adjust the command
    angleCommand = Pk_angle * angleError * accelerationFactor;
}

void MovementController::calculateDistanceCommand(double dX, double dY) {
    double distanceError = sqrt(dX * dX + dY * dY);
    distanceCommand = distanceError * Pk_distance * accelerationFactor;
    distanceCommand = MathUtils::constrain(distanceCommand, 0, 50);
}
