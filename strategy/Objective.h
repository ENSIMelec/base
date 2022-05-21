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
    explicit Objective(const char * name);

    void setOptional(bool anOptional);
    void addPoint(Point * point);

    void logInfos();

    [[nodiscard]] Point *getNextPoint();
    [[nodiscard]] int getPointIndex() const {return currentPointIndex;}
    [[nodiscard]] const char * getName() const {return name;}
    [[nodiscard]] Point * getCurrentPoint() {return currentPoint;}
    [[nodiscard]] int getNbPoints() const {return (int) points.size();}
private:
    const char * name = nullptr;
    bool optional = false;

    int currentPointIndex = 0;
    Point * currentPoint{};
    vector<Point *> points;
};

#endif //OBJECTIVE_H
