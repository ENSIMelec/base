//
// Created by Tom on 29/04/2022.
//

#include "Objective.h"

#include <utility>

Objective::Objective(string name) {
    this->name = std::move(name);
}

void Objective::setOptional(bool optional) {
    this->optional = optional;
}

void Objective::addPoint(Point * point) {
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
