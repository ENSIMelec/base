//
// Created by Tom on 15/05/2022.
//

#ifndef KONSOLE_SERIAL_CODER_MANAGER_WINDOW_H
#define KONSOLE_SERIAL_CODER_MANAGER_WINDOW_H


#include "ComponentWindow.h"
#include "../utility/Initializer.h"

class SerialCoderManagerWindow : public ComponentWindow {
public:
    SerialCoderManagerWindow(int x, int y, int w, int h) : ComponentWindow(x, y, w, h, "Serial Coder Manager") {
        serialCoderManager = Initializer::getSerialCoderManager();
    };
    void display() override;

private:
    SerialCoderManager *serialCoderManager;
};


#endif //KONSOLE_SERIAL_CODER_MANAGER_WINDOW_H
