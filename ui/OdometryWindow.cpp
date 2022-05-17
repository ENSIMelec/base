//
// Created by Tom on 15/05/2022.
//

#include "OdometryWindow.h"
#include "../utility/Initializer.h"

void OdometryWindow::display() {
    mvwprintw(win, 1, 1, "X: %.0fmm  Y: %.0fmm", odometry->getX(), odometry->getY());
    mvwprintw(win, 2, 1, "θ: %.5f (%.2f°)", odometry->getTheta(), MathUtils::rad2deg(odometry->getTheta()));
    mvwprintw(win, 3, 1, "Lin vel : %.2f   Ang vel : %.2f", odometry->getLinVel(), odometry->getAngVel());
}

OdometryWindow::OdometryWindow(int x, int y, int width, int height) :
    ComponentWindow(x, y, width, height, "Odometry")
{
    odometry = Initializer::getOdometry();
}

