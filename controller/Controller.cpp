#include "Controller.h"

#define DEBUG_CONTROLLER
//#define CSV_PRINT

#define ANGLE_THRESHOLD 2   // In degrees

Controller::Controller(MotorManager * motorManager_, Odometry * odometry_, Configuration * config_) {
    this->odometry = odometry_;
    this->motorManager = motorManager_;
    this->config = config_;

    movementController = new MovementController(config_);

//    Pk_angle = config->getDouble("controller.translation.Pk_angle");
//    Pk_distance = config->getDouble("controller.translation.Pk_distance");
//    accelerationCoeff = config->getDouble("controller.acceleration_coeff");
}

void Controller::update() {

    Location currentLocation = odometry->getLocation();
    double x = currentLocation.x;
    double y = currentLocation.y;
    double theta = currentLocation.theta;

    switch (currentPoint->getType()) {
        case PointType::MOVE_TO_POSITION:
            movementController->calculateCommands(x, y, theta);

            command.angle = movementController->getAngleCommand();
            command.distance = movementController->getDistanceCommand();
            break;
    }

//    calculateAngleCommands();
//    correctAngle();

    pwm.left = (int) (command.distance + command.angle);
    pwm.right = (int) (command.distance - command.angle);

    // Set the orders
    motorManager->setOrder(pwm.left, pwm.right);
}

void Controller::debug() {
#ifdef CSV_PRINT
    cout << consign.angle << ";" << odometry->getTheta() << endl;
#endif

#ifdef DEBUG_CONTROLLER
    Location pos = odometry->getLocation();

    double theta = odometry->getTheta();
    double distanceError = calculateDistanceError();
    double angleError = targetAngle - odometry->getTheta();

    command.angle = angleError;

    cout << "[POSITION] X: " << pos.x << "\tY: " << pos.y << "\tTheta: " << theta << "(" << MathUtils::rad2deg(theta) << "°)" << endl;
    cout << "[CONTROLLER] Distance Error : " << distanceError << "\tAngle error : " << angleError << " (" << MathUtils::rad2deg(angleError) << ")" << endl;

    cout << "[CONTROLLER] Angle error : " << angleError << endl;
    cout << "[CONTROLLER] Angle command : " << command.angle << "\tDistance command : " << command.distance << endl;
    cout << "[CONTROLLER] PWM Left : " << pwm.left << "\tPWM Right : " << pwm.right << endl;
#endif
}

void Controller::calculateDistanceCommands() {
    double distanceError = calculateDistanceError();

    double cmd = distanceError * Pk_distance * accelerationFactor;
    cmd = MathUtils::constrain(cmd, 0, 70);

    command.distance = cmd;
    cout << "[Calculate Distance Commands] Command : " << command.distance << endl;
}

void Controller::calculateAngleCommands() {
    double angleError = targetAngle - odometry->getTheta();
//    double angleError = calculateAngleError();    // Needed in continuous ?

    command.angle = angleError * Pk_angle * accelerationFactor;
    cout << "[Calculate Angle Commands] Command Left : " << - command.angle << "\tCommand Right : " << command.angle << endl;
}

void Controller::setTargetXY(int x, int y) {
    targetPosition.x = x;
    targetPosition.y = y;

    targetAngle = calculateAngleError();
    angleIsCorrect = false;

    command.angle = 0;
    command.distance = 0;
}

void Controller::stopMotors() {
    motorManager->stop();
}

void Controller::setNextPoint(Point * point) {
    currentPoint = point;

    switch (point->getType()) {
        case PointType::MOVE_TO_POSITION:
            movementController->setTargetPosition(point->getX(), point->getY());
            break;
    }
}

bool Controller::isTargetReached() {
    switch (currentPoint->getType()) {
        case PointType::MOVE_TO_POSITION:
            return movementController->isTargetReached();
    }

    return true;
}

double Controller::calculateAngleError() {

    // Calculate the angle error
    Location currentPosition = odometry->getLocation();
    double dX = targetPosition.x - currentPosition.x;
    double dY = targetPosition.y - currentPosition.y;

    double angleError = atan(dY / dX);
//    double sign = (targetPosition.y > currentPosition.y) ? -1 : 1;
//    double angleError = sign * acos(dX / (dX * dX + dY * dY));

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

    cout << "[ANGLE CALCULATION] dx : " << dX << " dy : " << dY << endl;
    cout << "[ANGLE CALCULATION] Angle error : " << angleError << " (" << MathUtils::rad2deg(angleError) << "°)" << endl;
//    cout << "[ANGLE CALCULATION] distance : " << sqrt(dX * dX + dY * dY) << endl;


    return angleError - currentPosition.theta;
}

/**
 * Get the distance between the current location and target
 */
double Controller::calculateDistanceError() {
    Location currentPosition = odometry->getLocation();

    double dX = currentPosition.x - targetPosition.x;
    double dY = currentPosition.y - targetPosition.y;

    return sqrt(dX * dX + dY * dY);
}

void Controller::correctAngle() {
    double angleError = calculateAngleError();

    command.angle = Pk_angle * angleError * accelerationFactor;
}


