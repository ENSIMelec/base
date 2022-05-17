#include "PID.h"
using namespace std;
/**
 * Construction PID
 * @param kp
 * @param ki
 * @param kd
 * @param min
 * @param max
 */
PID::PID(double kp, double ki, double kd, double min, double max) {

    this->m_kp = kp;
    this->m_ki = ki;
    this->m_kd = kd;
    this->m_min = min;
    this->m_max = max;
}
/**
 * @brief Calculer le PID
 *
 * @param currentState L'état actuelle
 * @param consigne  Goal
 * @param timestep
 * @return
 */
double PID::compute(double error) {

    // Proportional term
    double proportionalOut = m_kp * error;

    // Integral term
    this->m_integral += error;
    double integralOut = m_ki * this->m_integral;

    // Derivative term
    this->m_derivative = error - m_pre_error;
    double derivativeOut = m_kd * this->m_derivative;

    // Compute the PID controller's output
    double output = proportionalOut + integralOut + derivativeOut;

    // Restrict to max/min
    if(output > m_max )
        output = m_max;
    else if(output < m_min )
        output = m_min;

    // Save error to previous error
    this->m_pre_error = error;

    // Returns the manipulated variable given a set point and current process value
    return output;
}

void PID::setTunings(float kp, float ki, float kd) {
    if (kp < 0 || ki < 0 || kd < 0)
        return;

    this->m_kp = kp;
    this->m_ki = ki;
    this->m_kd = kd;
}

void PID::reset() {
    this->m_pre_error = 0;
    this->m_integral = 0;
    this->m_derivative = 0;
}

