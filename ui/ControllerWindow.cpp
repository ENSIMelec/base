//
// Created by Tom on 15/05/2022.
//

#include "ControllerWindow.h"
#include "../utility/Initializer.h"
#include <curses.h>

ControllerWindow::ControllerWindow(int x, int y, int width, int height) :
    ComponentWindow(x, y, width, height, "Controller")
{
    controller = Initializer::getController();
}

void ControllerWindow::display() {
    currentPoint = controller->getCurrentPoint();
    pointType = currentPoint->getType();

    mvwprintw(win, 1, 1, "Point type : %s   ", getPointTypeStr());
    displayPointInformation();

    mvwprintw(win, 6, 1, "Dist cmd : %.0f   Angle cmd : %.0f   ", controller->getDistCommand(), controller->getAngleCommand());
    mvwprintw(win, 7, 1, "PWM left : %d   PWM right : %d   ", controller->getPWMLeft(), controller->getPWMRight());
}

void ControllerWindow::displayPointInformation() {
    double angleError = controller->getAngleError();
    double angleError_deg = MathUtils::rad2deg(angleError);
    double distanceError = controller->getDistanceError();

    switch (pointType) {
        case PointType::MOVE_TO_POSITION:
        case PointType::MOVE_TO_POSITION_RELATIVE_TO_ANGLE:
        case STATIC_POSITION:
            mvwprintw(win, 2, 1, "X: %.0f   Y: %.0f   ", currentPoint->getX(), currentPoint->getY());
            mvwprintw(win, 3, 1, "Angle error : %.2f (%.2f)  ", angleError, angleError_deg);
            mvwprintw(win, 4, 1, "Dist error : %.0f   ", distanceError);
            break;
        case ABSOLUTE_ANGLE:
        case ANGLE:
            mvwprintw(win, 2, 1, "θ: %.5f (%.2f°)  ", currentPoint->getTheta(), MathUtils::rad2deg(currentPoint->getTheta()));
            mvwprintw(win, 3, 1, "Angle error : %.2frad  (%.2f°)  ", angleError, angleError_deg);
            break;
        case SET_X:
        case SET_Y:
        case SET_XY_THETA:
            break;
        case WAIT:
            mvwprintw(win, 2, 1, "Time : %f", currentPoint->getWaitingTime());
        default:
            return;
    }
}

const char *ControllerWindow::getPointTypeStr() {
    switch (pointType) {
        case PointType::MOVE_TO_POSITION:
            return "Move to position";
        case PointType::ABSOLUTE_ANGLE:
            return "Absolute angle";
        case STATIC_POSITION:
            return "Static position";
        case RELATIVE_ANGLE:
            return "Relative angle";
        case SET_X:
            return "Set X";
        case SET_Y:
            return "Set Y";
        case SET_XY_THETA:
            return "Set X, Y & Theta";
        case PointType::MOVE_TO_POSITION_RELATIVE_TO_ANGLE:
            return "Move relative to angle";
        case PointType::WAIT:
            return "Wait";
        case PointType::ANGLE:
            return "Angle";
        default:
            return "Undefined";
    }

    return "Undefined";
}
