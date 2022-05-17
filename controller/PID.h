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

    double compute(double error);
    void setTunings(float kp, float ki, float kd);
    void reset();

private:
    double m_kp; /** Proportional gain */
    double m_ki; /** Integral gain */
    double m_kd; /** derivative gain */

    double m_min; /** Minimum value of manipulated variable */
    double m_max; /** Maximum value of manipulated variable */

    double m_integral{};
    double m_derivative{};
    double m_pre_error{};
};


#endif //ROBOT_PID_H
