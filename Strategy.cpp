//
// Created by Tom on 23/02/2022.
//

#include "Strategy.h"
#include "StrategyParser.h"

Strategy::Strategy(const string& path, const string& name) {
    objectives = StrategyParser::readObjectives(path, name);

    if(StrategyParser::getStatus() == EXIT_SUCCESS) {
        cout << "Successfully loaded the strategy !" << endl;
    } else {
        cout << "Unable to load the strategy ..." << endl;
        return;
    }
}

void Strategy::logObjectives() {
    int i = 0;
    for(auto objective : *objectives) {
        cout << "[" << i++ << "]:: ";
        objective->logInfos();
    }
}