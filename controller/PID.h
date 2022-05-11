//
// Created by Taoufik on 11/11/2019.
//


#ifndef ROBOT_PID_H
#define ROBOT_PID_H

#include <cmath>
#include "iostream"

class PID {
public:


    PID(): m_kp(0), m_ki(0), m_kd(0), m_min(0), m_max(0) {};
    PID(double kp, double ki, double kd, double min, double max);

    double compute(float input, float consigne);
    void setTunings(float kp, float ki, float kd);
    void resetErrors();

    float getCurrentGoal() const;
    void setGoal(float mGoal);
    int getError() const { return m_pre_error; };


    double getDeltaError();

private:
    double m_goal;

    // m_kp -  proportional gain
    // m_ki -  Integral gain
    // m_kd -  derivative gain
    // timestep -  loop interval time

    double m_kp;
    double m_ki;
    double m_kd;

    // m_min - maximum value of manipulated variable
    // m_max - minimum value of manipulated variable
    double m_min, m_max;

    double m_pre_error   = 0;
    double m_derivative  = 0;
    double m_integral    = 0;

};


#endif //ROBOT_PID_H
