//
// Created by Tom on 29/04/2022.
//

#include <iostream>
using namespace std;

#include "Objective.h"

Objective::Objective(string name) {
    this->name = std::move(name);
}

void Objective::setOptional(bool optional) {
    this->optional = optional;
}

void Objective::addPoint(Point * point) {
    // Setting the current point if it is the first
    if(points.empty()) currentPoint = point;

    points.push_back(point);
}

void Objective::logInfos() {
    cout << name << endl;
    if(optional) {
        cout << "Optional" << endl;
    }

    cout << "Points :" << endl;
    for(int i = 0; i < points.size(); i++) {
        Point * p = points[i];

        cout << "[" << i << "]::" << endl;
        cout << "\tX:" << p->getX() << endl;
        cout << "\tY:" << p->getY() << endl;
        cout << "\tTheta: " << p->getTheta() << endl;
        cout << "\tType: " << p->getType() << endl;
    }
}

Point *Objective::getNextPoint() {

    currentPointIndex++;
    if(currentPointIndex >= points.size()) {
        // There is no more points
        done = true;
        return nullptr;
    } else {
        // Return the next point in the objective
        currentPoint = points.at(currentPointIndex);
        return currentPoint;
    }
}

