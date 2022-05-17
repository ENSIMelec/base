//
// Created by Tom on 15/05/2022.
//

#include "SerialCoderManagerWindow.h"

#include <curses.h>

void SerialCoderManagerWindow::display() {
    mvwprintw(win, 1, 1, "Lticks: %d   Rticks: %d   Δt: %.0fms", serialCoderManager->getLeftTicks(),
              serialCoderManager->getRightTicks(), serialCoderManager->getTime());
}
