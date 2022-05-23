//
// Created by Tom on 08/05/2022.
//

#include <math.h>

#include "MovementController.h"
#include "../utility/MathUtils.h"

MovementController::MovementController(Configuration * configuration) {
    acceleration = configuration->getDouble("controller.movement.acceleration");

    double Pk_distance = configuration->getDouble("controller.movement.distance.Pk");
    double Pi_distance = configuration->getDouble("controller.movement.distance.Pi");
    double Pd_distance = configuration->getDouble("controller.movement.distance.Pd");

    double Pk_angle = configuration->getDouble("controller.movement.angle.Pk");
    double Pi_angle = configuration->getDouble("controller.movement.angle.Pi");
    double Pd_angle = configuration->getDouble("controller.movement.angle.Pd");

    double minDistancePWM = configuration->getDouble("controller.movement.distance.min_pwm");
    double maxDistancePWM = configuration->getDouble("controller.movement.distance.max_pwm");
    double minAnglePWM = configuration->getDouble("controller.movement.angle.min_pwm");
    double maxAnglePWM = configuration->getDouble("controller.movement.angle.max_pwm");

    distanceThreshold = configuration->getDouble("controller.movement.distance_threshold");

    originalMaxSpeed = maxDistancePWM;

    pid_distance = new PID(Pk_distance, Pi_distance, Pd_distance, minDistancePWM, maxDistancePWM);
    pid_angle = new PID(Pk_angle, Pi_angle, Pd_angle, minAnglePWM, maxAnglePWM);
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
    if(distanceError > 5 * distanceThreshold) {
        calculateAngleCommand(dX, dY, theta);
    } else {
        angleCommand = 0;
    }

    // Check if the target is reached
    double distance = sqrt(dX * dX + dY * dY);
    if(distance < distanceThreshold) {
        targetReached = true;
    }

    // Check if timeout is active
    if(timeout > 0) {
        if(timer.elapsed_ms() > (unsigned long long) timeout) {
            targetReached = true;
            timeout = -1;   // Disable the timeout for the next point
        }
    }
}

void MovementController::setTargetPosition(double x, double y) {
    targetLocation.x = x;
    targetLocation.y = y;

    accelerationFactor = 0;

    targetReached = false;
    resetMaxSpeed();

    pid_angle->reset();
    pid_distance->reset();

    direction = FORWARD;

    timer.restart();
}

void MovementController::calculateAngleCommand(double dX, double dY, double theta) {

    const double base = M_PI;
    double dest_angle = calculateDestinationAngle(dX, dY);

    // In case we are backward, we do the same calculations as forward but with opposite angle
    if(direction == BACKWARD) {
        theta += base;
    }

    theta = MathUtils::normalizeAngle(theta);

    double alpha_r = 0, alpha_d = 0;
    if (theta < 0) {
        alpha_r = -base - theta;
    } else {
        alpha_r = base - theta;
    }

    if (dest_angle < 0) {
        alpha_d = base + dest_angle;
    } else {
        alpha_d = -base + dest_angle;
    }

    double s1 = dest_angle - theta;
    double s2 = alpha_r + alpha_d;

    angleError = (abs(s1) < abs(s2)) ? s1 : s2;

//    cout << "[ABSOLUTE_ANGLE CALCULATION] Angle error : " << angleError << " (" << MathUtils::rad2deg(angleError) << "°)" << endl;

    // Adjust the command
    angleCommand = pid_angle->compute(angleError);
    angleCommand *= accelerationFactor;
}

void MovementController::calculateDistanceCommand(double dX, double dY) {
    distanceError = sqrt(dX * dX + dY * dY);
    distanceCommand = pid_distance->compute(distanceError);
    distanceCommand *= accelerationFactor;
}

double MovementController::calculateDestinationAngle(double dX, double dY) {

    if(dX == 0) return 0;

    double angleError = atan(dY / dX);
    if(dX < 0) {
        if (dY > 0) angleError += M_PI;
        if (dY < 0) angleError -= M_PI;
    }

    // Stay in the range -Pi, Pi
    if(angleError < - M_PI) {
        angleError += M_2_PI;
    } else if(angleError > M_PI) {
        angleError -= M_2_PI;
    }

    return angleError;
}

void MovementController::setMaxSpeed(double max) {
    pid_distance->setMinMax(-max, max);
}

void MovementController::resetMaxSpeed() {
    pid_distance->setMinMax(-originalMaxSpeed, originalMaxSpeed);
}
