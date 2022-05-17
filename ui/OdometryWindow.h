//
// Created by Tom on 15/05/2022.
//

#ifndef KONSOLE_ODOMETRY_WINDOW_H
#define KONSOLE_ODOMETRY_WINDOW_H

#include <curses.h>
#include "ComponentWindow.h"
#include "../odometry/Odometry.h"

class OdometryWindow : public ComponentWindow {

public:
    OdometryWindow(int x, int y, int width, int height);
    void display() override;

private:
    Odometry *odometry;
};


#endif //KONSOLE_ODOMETRY_WINDOW_H
