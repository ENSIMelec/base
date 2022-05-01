//
// Created by Tom on 23/02/2022.
//

#include <iostream>
using namespace std;

#include "Strategy.h"
#include "StrategyParser.h"

Strategy::Strategy(const string& path, const string& name) {
    objectives = StrategyParser::readObjectives(path, name);

    if(StrategyParser::getStatus() == EXIT_SUCCESS) {
        cout << "Successfully loaded the strategy with " << objectives->size() << " objectives !" << endl;
    } else {
        cout << "Unable to load the strategy ..." << endl;
        return;
    }

    // Get the current objective
    if(!objectives->empty()) {
        currentObjective = objectives->at(0);
    } else {
        strategyIsDone = true;
    }
}

void Strategy::logObjectives() {
    int i = 0;
    for(auto objective : *objectives) {
        cout << "[" << i++ << "]:: ";
        objective->logInfos();
    }
}

Point * Strategy::getNextPoint() {
    Point * point = currentObjective->getNextPoint();

    if(currentObjective->isDone()) {
        currentObjectiveIndex++;

        // Check if we are at the end of the objective list
        if(currentObjectiveIndex >= objectives->size()) {
            strategyIsDone = true;
            return nullptr;
        } else {
            // Go to the next objective
            currentObjective = objectives->at(currentObjectiveIndex);
            point = currentObjective->getNextPoint();

            cout << "Started a new objective : " << currentObjective->getName() << endl;
        }
    }

    point->logTargetInformation();

    return point;
}

Point * Strategy::getCurrentPoint() {
    return currentObjective->getCurrentPoint();
}
