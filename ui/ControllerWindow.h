//
// Created by Tom on 15/05/2022.
//

#ifndef KONSOLE_CONTROLLER_WINDOW_H
#define KONSOLE_CONTROLLER_WINDOW_H

#include "ComponentWindow.h"
#include "../strategy/PointType.h"
#include "../controller/Controller.h"

class ControllerWindow : public ComponentWindow {
public:
    ControllerWindow(int x, int y, int width, int height);
    void display() override;

private:
    double x = 563, y = -843;

    double angle = 0.86254, angle_deg = 32.652;
    double angle_error = 0.86254, angle_error_deg = 32.652;
    double dist_error = 263.58;

    int dist_cmd = -53, angle_cmd = 36;
    int pwm_left = -99, pwm_right = -23;


    void displayPointInformation();

    const char *getPointTypeStr();

    PointType pointType = PointType::MOVE_TO_POSITION;
    Point * currentPoint;
    Controller *controller;
};


#endif //KONSOLE_CONTROLLER_WINDOW_H
