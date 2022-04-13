//
// Created by Taoufik on 20/12/2019.
//

#include "Point.h"

Point::Point(float x, float y, float theta) :
    m_x(x),
    m_y(y),
    m_theta(theta) {

}

float Point::getX() const {
    return m_x;
}

float Point::getY() const {
    return m_y;
}

float Point::getTheta() const {
    return m_theta;
}

float Point::getMDistanceTolerance() const {
    return m_distance_tolerance;
}

void Point::setMDistanceTolerance(float mDistanceTolerance) {
    m_distance_tolerance = mDistanceTolerance;
}

float Point::getMAngleTolerance() const {
    return m_angle_tolerance;
}

void Point::setMAngleTolerance(float mAngleTolerance) {
    m_angle_tolerance = mAngleTolerance;
}

int Point::getMSpeed() const {
    return m_speed;
}

void Point::setMSpeed(int mSpeed) {
    m_speed = mSpeed;
}


int Point::getMTimeout() const {
    return m_timeout;
}

void Point::setMTimeout(int mTimeout) {
    m_timeout = mTimeout;
}