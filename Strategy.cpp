//
// Created by Tom on 23/02/2022.
//

#include "Strategy.h"

Strategy::Strategy(Point * startingPoint) {
    cout << "Starting position : " << endl;
    cout << "\tP (" << startingPoint->getX() << ", " << startingPoint->getY() << ", " << startingPoint->getTheta() << ")" << endl;

    addPoint(startingPoint);

    m_currentPoint = startingPoint;
}

Point * Strategy::getCurrentPoint() {
    return this->m_currentPoint;
}

bool Strategy::isDone() {
    return this->m_isDone;
}

void Strategy::addPoint(Point * point) {
    m_points.push_back(point);
}

void Strategy::setNextPoint(Controller *controller) {
    m_currentPointIndex++;
    if(m_currentPointIndex >= m_points.size()) {
        cout << "Strategy is done !" << endl;
        m_isDone = true;
        return;
    }

    m_currentPoint = m_points.operator[](m_currentPointIndex);

    cout << "Setting next point : (" << m_currentPoint->getX() << ", " << m_currentPoint->getY() << ", " <<
        m_currentPoint->getTheta() << ")" << endl;

    controller->setTargetXY(m_currentPoint->getX(), m_currentPoint->getY());
}

void Strategy::initController(Controller *controller) {
    cout << "==========================" << endl;
    cout << "Strategy points : " << endl;
    int i = 1;
    for(Point * point : m_points) {
        cout << "\tP" << i++ << " (" << point->getX() << ", " << point->getY() << ", " << point->getTheta() << ")" << endl;
    }

    // Initialize the position
    Point * firstPoint = m_points.operator[](0);

    setNextPoint(controller);
}
