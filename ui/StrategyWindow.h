//
// Created by Tom on 15/05/2022.
//

#ifndef KONSOLE_STRATEGY_WINDOW_H
#define KONSOLE_STRATEGY_WINDOW_H


#include "ComponentWindow.h"
#include "../strategy/Strategy.h"

class StrategyWindow : public ComponentWindow {
public:
    StrategyWindow(int x, int y, int width, int height);
    void display() override;

private:
    const char* strategy_name = "MAKING CAKE";
    const char* current_objective_name = "Getting the ingredients";
    Strategy * strategy;
};


#endif //KONSOLE_STRATEGY_WINDOW_H
