//
// Created by Tom on 29/04/2022.
//

#ifndef OBJECTIVE_H
#define OBJECTIVE_H

// Includes
#include <string>
#include <vector>

#include "Point.h"

class Objective {

public:
    Objective() = default;
    explicit Objective(string name);

    void setOptional(bool anOptional);
    void addPoint(Point * point);

    void logInfos();

    Point *getNextPoint();
    bool isDone() const {return done;}
    string getName() const {return name;}

    Point * getCurrentPoint() {return currentPoint;}
private:
    string name;
    bool optional = false;

    bool done = false;

    int currentPointIndex = 0;
    Point * currentPoint{};
    vector<Point *> points;
};

#endif //OBJECTIVE_H
