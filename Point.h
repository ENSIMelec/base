//
// Created by Taoufik on 20/12/2019.
//

#ifndef ROBOT_POINT_H
#define ROBOT_POINT_H

#include "Controller.h"
#include <string>

using namespace std;

class Point {

public:
    Point() = default;
    Point(float mX, float mY, float mTheta);

    // ----- Getters & Setters -----
    float getX() const {return m_x;}
    void setX(float x) {m_x = x;}

    float getY() const {return m_y;}
    void setY(float y) {m_y = y;}

    float getTheta() const {return m_theta;}
    void setTheta(float theta) {m_theta = theta;}

    float getDistanceTolerance() const {return m_distance_tolerance;}
    void setDistanceTolerance(float distance_tolerance) {m_distance_tolerance = distance_tolerance;}

    float getAngleTolerance() const {return m_angle_tolerance;}
    void setAngleTolerance(float mAngleTolerance) {m_angle_tolerance = mAngleTolerance;}

    int getSpeed() const {return m_speed;}
    void setSpeed(int mSpeed) {m_speed = mSpeed;}

    int getTimeout() const {return m_timeout;}
    void setTimeout(int mTimeout) {m_timeout = mTimeout;}

    void setAction(const string& action) {m_action = action;}
    string getAction() {return m_action;}

    void setDirection(string direction) {m_direction = direction;}

    void setActionWaiting(bool wait) {m_waitForAction = wait;}
    void setBlocage(string blocked) {m_blocked = blocked;}
    void setCommentary(string commentary) {m_commentary = commentary;}

    string getType() {return m_type;}
    void setType(string type) {m_type = type;}

private:
    float m_x{};
    float m_y{};
    float m_theta{};
    float m_distance_tolerance{};
    float m_angle_tolerance{};
    int m_speed{};
    int m_timeout{};
    string m_action = {};
    bool m_waitForAction = false;
    string m_blocked;
    string m_commentary;
    string m_type;
    string m_direction;
};


#endif //ROBOT_POINT_H
