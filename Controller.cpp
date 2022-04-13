#include "Controller.h"

#define DEBUG_CONTROLLER
//#define CSV_PRINT

void Controller::update() {

    double distance = calculateDistanceError() * Pk_distance;
    int distanceCommand = MathUtils::constrain(distance, 0, 100);

    double angleError = calculateAngleError();
    double angleCommand = angleError * Pk_angle;

    // Apply commands
    motorManager->setOrder(distanceCommand - angleCommand, distanceCommand + angleCommand);
}

void Controller::debug() {
#ifdef CSV_PRINT
    cout << consign.angle << ";" << odometry->getTheta() << endl;
#endif

#ifdef DEBUG_CONTROLLER
    Position pos = odometry->getPosition();
    double distanceError = calculateDistanceError();
    double angleError = calculateAngleError();

    cout << "[POSITION] X: " << pos.x << "\tY: " << pos.y << "\tTheta: " << pos.theta << "(" << MathUtils::rad2deg(pos.theta) << "°)" << endl;
    cout << "[CONTROLLER] Distance Error : " << distanceError << "\tAngle error : " << angleError << endl;

    cout << "[CONTROLLER] Angle error : " << angleError << endl;
    cout << "[CONTROLLER] Angle command : " << command.angle << "\tDistance command : " << command.distance << endl;
#endif
}

void Controller::absoluteAngle() {
    double angleError = calculateAngleError();
    command.angle = angleError * Pk_angle;
    command.distance = 0;

    currentTrajectoryType = ANGLE;
}

void Controller::setTargetAngle(double angle) {
    targetPosition.theta = angle;
}

void Controller::setTargetXY(int x, int y) {
    targetPosition.x = x;
    targetPosition.y = y;
}

void Controller::stopMotors() {
    motorManager->stop();
}

bool Controller::isTargetReached() {

    // Get the distance between actual position and target
    double distance = calculateDistanceError();
    return distance < DISTANCE_TRESHOLD;
}

double Controller::calculateAngleError() {

    // Calculate the angle error
    Position currentPosition = odometry->getPosition();
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
double Controller::calculateDistanceError() {
    Position currentPosition = odometry->getPosition();

    double dX = currentPosition.x - targetPosition.x;
    double dY = currentPosition.y - targetPosition.y;

    return sqrt(dX * dX + dY * dY);
}
