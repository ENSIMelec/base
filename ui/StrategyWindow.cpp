//
// Created by Tom on 15/05/2022.
//

#include "StrategyWindow.h"
#include <curses.h>
#include "../utility/Initializer.h"

StrategyWindow::StrategyWindow(int x, int y, int width, int height) :
    ComponentWindow(x, y, width, height, "Strategy") {
    strategy = Initializer::getStrategy();
}

void StrategyWindow::display() {
    mvwprintw(win, 1, 1, "Name : %s", strategy->getName());
    mvwprintw(win, 2, 1, "Objective : %s", strategy->getCurrentObjective()->getName());

    mvwprintw(win, 4, 1, "Point %d/%d",
              strategy->getCurrentObjective()->getPointIndex() + 1,
              strategy->getCurrentObjective()->getNbPoints());
}
