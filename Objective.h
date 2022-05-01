//
// Created by Tom on 29/04/2022.
//

#ifndef KRABBS_OBJECTIVE_H
#define KRABBS_OBJECTIVE_H

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

private:
    string name;
    bool optional = false;

    vector<Point *> points;
};


#endif //KRABBS_OBJECTIVE_H
