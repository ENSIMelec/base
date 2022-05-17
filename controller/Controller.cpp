#include "Controller.h"

#define DEBUG_CONTROLLER
//#define CSV_PRINT

#define ANGLE_THRESHOLD 2   // In degrees

Controller::Controller(MotorManager * motorManager_, Odometry * odometry_, Configuration * config_) {
    this->odometry = odometry_;
    this->motorManager = motorManager_;
    this->config = config_;

    activateMotors = config_->getInt("global.activate_motors");

    movementController = new MovementController(config_);
    angleController = new AngleController(config_);

//    Pk_angle = config->getDouble("controller.translation.Pk_angle");
//    Pk_distance = config->getDouble("controller.translation.Pk_distance");
//    accelerationCoeff = config->getDouble("controller.acceleration_coeff");
}

void Controller::update() {

    command.distance = 0;
    command.angle = 0;

    Location currentLocation = odometry->getLocation();
    double x = odometry->getX();
    double y = odometry->getY();
    double theta = odometry->getTheta();

    switch (currentPoint->getType()) {
        case PointType::MOVE_TO_POSITION:
            movementController->calculateCommands(x, y, theta);

            command.angle = movementController->getAngleCommand();
            command.distance = movementController->getDistanceCommand();
            break;
        case PointType::ANGLE:
            angleController->calculateCommands(theta);

            command.angle = angleController->getAngleCommand();
            break;
    }

    pwm.left = (int) (command.distance - command.angle);
    pwm.right = (int) (command.distance + command.angle);

    // Set the orders
    if(activateMotors != 0) {
        motorManager->setOrder(pwm.left, pwm.right);
    }
    else {
//        cout << "Warning : Motors are deactivated !" << endl;
    }
}

void Controller::debug() {
#ifdef CSV_PRINT
    cout << consign.angle << ";" << odometry->getTheta() << endl;
#endif

#ifdef DEBUG_CONTROLLER
    Location pos = odometry->getLocation();

    cout << "[POSITION] X: " << pos.x << "\tY: " << pos.y << "\tTheta: " << pos.theta << "(" << MathUtils::rad2deg(pos.theta) << "°)" << endl;
    cout << "[CONTROLLER][Commands] Distance : " << command.distance << "\tAngle : " << command.angle << endl;
    cout << "[CONTROLLER][PWM] Left : " << pwm.left << "\tRight : " << pwm.right << endl;
#endif
}

void Controller::stopMotors() {
    pwm.left = 0;
    pwm.right = 0;

    command.angle = 0;
    command.distance = 0;

    motorManager->stop();
}

void Controller::setNextPoint(Point * point) {
    currentPoint = point;

    switch (point->getType()) {
        case PointType::MOVE_TO_POSITION:
            movementController->setTargetPosition(point->getX(), point->getY());
            break;
        case PointType::ANGLE:
            angleController->setTargetAngle(point->getTheta());
            break;
    }
}

bool Controller::isTargetReached() {
    switch (currentPoint->getType()) {
        case PointType::MOVE_TO_POSITION:
            return movementController->isTargetReached();
        case PointType::ANGLE:
            return angleController->isTargetReached();
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

double Controller::getDistanceError() {
    switch (currentPoint->getType()) {
        case POSITION:
            break;
        case MOVE_TO_POSITION:
            return movementController->getDistanceError();
            break;
        case RECALAGE_X:
            break;
        case RECALAGE_Y:
            break;
        case RECALAGE_XY:
            break;
        default:
            return 0;
    }

    return 0;
}

double Controller::getAngleError() {
    switch (currentPoint->getType()) {
        case POSITION:
            break;
        case ANGLE:
            return angleController->getAngleError();
            break;
        case MOVE_TO_POSITION:
            return movementController->getAngleError();
            break;
        case RECALAGE_X:
            break;
        case RECALAGE_Y:
            break;
        case RECALAGE_XY:
            break;
        default:
            return 0;
    }

    return 0;
}

