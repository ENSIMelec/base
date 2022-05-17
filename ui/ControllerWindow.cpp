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

    mvwprintw(win, 1, 1, "Point type : %s            ", getPointTypeStr());
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
            mvwprintw(win, 2, 1, "X: %.0f   Y: %.0f   ", currentPoint->getX(), currentPoint->getY());
            mvwprintw(win, 3, 1, "Angle error : %.2f (%.2f)  ", angleError, angleError_deg);
            mvwprintw(win, 4, 1, "Dist error : %.0f   ", distanceError);
            break;
        case POSITION:
            break;
        case ANGLE:
            mvwprintw(win, 2, 1, "θ: %.5f (%.2f°)  ", currentPoint->getTheta(), MathUtils::rad2deg(currentPoint->getTheta()));
            mvwprintw(win, 3, 1, "Angle error : %.2frad  (%.2f°)  ", angleError, angleError_deg);
            break;
        case RECALAGE_X:
            break;
        case RECALAGE_Y:
            break;
        case RECALAGE_XY:
            break;
        default:
            return;
    }
}

const char *ControllerWindow::getPointTypeStr() {
    switch (pointType) {
        case PointType::MOVE_TO_POSITION:
            return "MoveToPosition";
        case PointType::ANGLE:
            return "Angle";
        case POSITION:
            return "Position";
        case ANGLE_RELATIF:
            break;
        case RECALAGE_X:
            break;
        case RECALAGE_Y:
            break;
        case RECALAGE_XY:
            break;
        default:
            return "Undefined";
    }

    return "Undefined";
}
