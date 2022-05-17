//
// Created by Tom on 15/05/2022.
//

#ifndef KONSOLE_ACTION_MANAGER_WINDOW_H
#define KONSOLE_ACTION_MANAGER_WINDOW_H


#include "ComponentWindow.h"

class ActionManagerWindow : public ComponentWindow {
public:
    ActionManagerWindow(int x, int y, int w, int h) : ComponentWindow(x, y, w, h, "Action Manager") {};
    void display() override;
};


#endif //KONSOLE_ACTION_MANAGER_WINDOW_H
