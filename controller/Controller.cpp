#include "Controller.h"

#include <cmath>
#include "../ui/UI.h"

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
        case PointType::MOVE_TO_POSITION_RELATIVE_TO_ANGLE:
            movementController->calculateCommands(x, y, theta);

            command.angle = movementController->getAngleCommand();
            command.distance = movementController->getDistanceCommand();

            if(currentPoint->getDirection() == MovementController::BACKWARD) {
                command.distance *= -1;
            }

            break;
        case PointType::ABSOLUTE_ANGLE:
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

    cout << "[STATIC_POSITION] X: " << pos.x << "\tY: " << pos.y << "\tTheta: " << pos.theta << "(" << MathUtils::rad2deg(pos.theta) << "°)" << endl;
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

    double xRobot = odometry->getX();
    double yRobot = odometry->getY();
    double thetaRobot = odometry->getTheta();
    int timeout = point->getTimeout();
    MovementController::Direction direction = point->getDirection();

    double xRelative, yRelative, x, y;

    switch (point->getType()) {
        case PointType::MOVE_TO_POSITION:
            movementController->setTargetPosition(point->getX(), point->getY());
            if(point->getMaxSpeed() > 0) movementController->setMaxSpeed(point->getMaxSpeed());
            if(timeout > 0) movementController->setTimeout(timeout);
            if(direction == MovementController::BACKWARD) movementController->setDirection(MovementController::BACKWARD);

            break;
        case PointType::MOVE_TO_POSITION_RELATIVE_TO_ANGLE:
            xRobot = odometry->getX();
            yRobot = odometry->getY();
            thetaRobot = odometry->getTheta();

            // The relative coordinates in the robot coordinate system relative to angle
            xRelative = point->getX();
            yRelative = point->getY();

            x = xRelative * cos(thetaRobot) - yRelative * sin(thetaRobot) + xRobot;
            y = xRelative * sin(thetaRobot) + yRelative * cos(thetaRobot) + yRobot;

            movementController->setTargetPosition(x, y);
            if(point->getMaxSpeed() > 0) movementController->setMaxSpeed(point->getMaxSpeed());
            if(timeout > 0) movementController->setTimeout(timeout);
            if(direction == MovementController::BACKWARD) movementController->setDirection(MovementController::BACKWARD);

            break;
        case PointType::ABSOLUTE_ANGLE:
            angleController->setAbsoluteTargetAngle(point->getTheta());
            break;
        case PointType::ANGLE:
            angleController->setTargetAngle(thetaRobot, point->getTheta());
            break;
        case PointType::RELATIVE_ANGLE:
            break;
        case PointType::SET_X:
            odometry->setX(point->getX());
            break;
        case PointType::SET_Y:
            odometry->setY(point->getY());
            break;
        case PointType::SET_XY_THETA:
            odometry->setPosition(point->getX(), point->getY(), point->getTheta());
            break;
        case STATIC_POSITION:
        case ACTION:
            break;
        case WAIT:
            delay((unsigned int) point->getWaitingTime());
            break;
        case SET_THETA:
            odometry->setTheta(point->getTheta());
            break;
    }
}

bool Controller::isTargetReached() {
    switch (currentPoint->getType()) {
        case MOVE_TO_POSITION:
        case MOVE_TO_POSITION_RELATIVE_TO_ANGLE:
            return movementController->isTargetReached();
        case ABSOLUTE_ANGLE:
            return angleController->isTargetReached();
        case WAIT:
        case ACTION:
        case SET_X:
        case SET_Y:
        case SET_THETA:
        case SET_XY_THETA:
            return true;
        default:
            return false;
    }

    return false;
}

double Controller::getDistanceError() {
    switch (currentPoint->getType()) {
        case STATIC_POSITION:
            break;
        case MOVE_TO_POSITION:
            return movementController->getDistanceError();
            break;
        case SET_X:
            break;
        case SET_Y:
            break;
        case SET_XY_THETA:
            break;
        default:
            return 0;
    }

    return 0;
}

double Controller::getAngleError() {
    switch (currentPoint->getType()) {
        case STATIC_POSITION:
            break;
        case ABSOLUTE_ANGLE:
            return angleController->getAngleError();
            break;
        case MOVE_TO_POSITION:
            return movementController->getAngleError();
            break;
        case SET_X:
            break;
        case SET_Y:
            break;
        case SET_XY_THETA:
            break;
        default:
            return 0;
    }

    return 0;
}

