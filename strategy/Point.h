//
// Created by Taoufik on 20/12/2019.
//

#ifndef POINT_H
#define POINT_H

#include <string>
#include <utility>
#include "PointType.h"
#include "../controller/MovementController.h"

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

    float getAngleTolerance() const {return m_angle_tolerance;}
    void setAngleTolerance(float mAngleTolerance) {m_angle_tolerance = mAngleTolerance;}

    int getSpeed() const {return speed;}
    void setSpeed(int mSpeed) { speed = mSpeed;}

    int getTimeout() const {return timeout;}
    void setTimeout(int mTimeout) { timeout = mTimeout;}

    void setAction(const string& p_action) { action = p_action;}
    string getAction() {return action;}

    MovementController::Direction getDirection() {return direction;}
    void setDirection(MovementController::Direction d) { direction = d;}

    void setActionAfterMovement(bool p_actionAfterMovement) { actionAfterMovement = p_actionAfterMovement;}
    bool isActionAfterMovement() const {return actionAfterMovement;}

    void setBlocage(string blocked) {m_blocked = std::move(blocked);}
    void setCommentary(string commentary) {m_commentary = std::move(commentary);}

    bool getLissage() {return lissage;}
    void setLissage(bool l) {lissage = l;}

    PointType getType() {return m_type;}
    void setType(PointType type) {m_type = type;}

    double getKpCurve() {return coeffCourbe;}
    void setKpCurve(double coef) { coeffCourbe = coef;}

    double getDeltaAngle() {return deltaAngle;}
    void setDeltaAngle(double delta) {deltaAngle = delta;}

    bool getSmoothCurve() {return smoothCurve;}
    void setSmoothCurve(bool p_smoothCurve) { smoothCurve = p_smoothCurve;}

    double getDistanceThreshold() {return distanceThreshold;}
    void setDistanceThreshold(double p_distanceThreshold) { distanceThreshold = p_distanceThreshold;}

    void setWaitingTime(double t) { waitingTime = t;}
    double getWaitingTime() const {return waitingTime;}

    void setLidarShouldBeActivated(bool d) { lidarShouldBeActivated = d;}
    bool getLidarShouldBeActivated() const {return lidarShouldBeActivated;}

    void logTargetInformation();

    void setMaxSpeed(double s) {maxSpeed = s;}
    double getMaxSpeed() const {return maxSpeed;}


    bool getChangeLidarState() {return changeLidarState; }
    void setChangeLidarState(bool state) { changeLidarState = state;}

private:
    /** Distance command max speed. -1 means default */
    double maxSpeed = -1;

    /** Timeout time. -1 means no timeout */
    int timeout = -1;

    /** For the waiting time, when type = "WAIT" */
    double waitingTime = 0;

    float m_x{};
    float m_y{};
    float m_theta{};
    float m_angle_tolerance{};
    int speed{};
    string action = "null";
    bool actionAfterMovement = true;
    string m_blocked;
    string m_commentary;
    PointType m_type;
    bool lidarShouldBeActivated = true;
    bool changeLidarState = false;

    MovementController::Direction direction = MovementController::FORWARD;

    bool lissage = false;
    double coeffCourbe = 0;
    double deltaAngle;
    bool smoothCurve;
    double distanceThreshold;
};


#endif //POINT_H
