//
// Created by Tom on 23/02/2022.
//

#ifndef KRABBS_STRATEGY_H
#define KRABBS_STRATEGY_H

#include <vector>
#include <string>
#include "Controller.h"
#include "Point.h"
#include "Objective.h"

using namespace std;

class Strategy {
public:
    Strategy(const string& path, const string& name);
    void logObjectives();

    bool isDone() const {return strategyIsDone;}
    Point * getNextPoint();

    Point *getCurrentPoint();

    Objective * getCurrentObjective() {return currentObjective;}

private:
    int currentObjectiveIndex = 0;
    Objective * currentObjective;
    vector<Objective *> * objectives;

    bool strategyIsDone = false;
};


#endif //KRABBS_STRATEGY_H
