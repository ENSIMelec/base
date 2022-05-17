//
// Created by Tom on 23/02/2022.
//

#ifndef KRABBS_STRATEGY_H
#define KRABBS_STRATEGY_H

#include <vector>
#include <string>
#include "../controller/Controller.h"
#include "Point.h"
#include "Objective.h"

using namespace std;

class Strategy {
public:
    Strategy(const string& path, const string& name);
    void logObjectives();

    // ---------- Getters ----------
    [[nodiscard]] bool isDone() const {return strategyIsDone;}
    [[nodiscard]] Point * getNextPoint();
    [[nodiscard]] Point *getCurrentPoint();
    [[nodiscard]] Objective * getCurrentObjective() {return currentObjective;}
    [[nodiscard]] const char * getName() const {return name;}

private:
    const char * name;
    int currentObjectiveIndex = 0;

    Objective * currentObjective;
    vector<Objective *> * objectives;

    bool strategyIsDone = false;
};


#endif //KRABBS_STRATEGY_H
