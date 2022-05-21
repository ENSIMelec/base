//
// Created by Tom on 23/02/2022.
//

#include <iostream>
using namespace std;

#include "Strategy.h"
#include "StrategyParser.h"

Strategy::Strategy(const string& path, const string& n) {
    name = n.c_str();
    objectives = StrategyParser::readObjectives(path + n + "/", "main.strategy");

    if(StrategyParser::getStatus() == EXIT_SUCCESS) {
//        cout << "Successfully loaded the strategy with " << objectives->size() << " objectives !" << endl;
    } else {
        printf("Unable to load the strategy ...\n");
        return;
    }

    // Get the current objective
    if(!objectives->empty()) {
        currentObjective = objectives->at(0);
    }
}

void Strategy::logObjectives() {
    int i = 0;
    for(auto objective : *objectives) {
        cout << "[" << i++ << "]:: ";
        objective->logInfos();
    }
}

/** Function that returns the next point of the strategy or a nullptr if the strategy has ended */
Point * Strategy::getNextPoint() {
    Point * point = currentObjective->getNextPoint();

    if(point == nullptr) {
        currentObjectiveIndex++;

        // Check if we are at the end of the objective list
        if(currentObjectiveIndex >= objectives->size()) {
            return nullptr;
        } else {
            // Go to the next objective
            currentObjective = objectives->at(currentObjectiveIndex);
            point = currentObjective->getNextPoint();

            cout << "Started a new objective : " << currentObjective->getName() << endl;
        }
    }

    return point;
}
Point * Strategy::getCurrentPoint() {
    return currentObjective->getCurrentPoint();
}
