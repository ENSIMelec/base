//
// Created by Tom on 15/05/2022.
//

#ifndef KONSOLE_INFORMATION_WINDOW_H
#define KONSOLE_INFORMATION_WINDOW_H


#include "ComponentWindow.h"

class InformationWindow : public ComponentWindow {
public:
    InformationWindow(int x, int y, int w, int h) : ComponentWindow(x, y, w, h, "Information") {};
    void display() override;
};


#endif //KONSOLE_INFORMATION_WINDOW_H
