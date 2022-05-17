//
// Created by Tom on 15/05/2022.
//

#include "ComponentWindow.h"

ComponentWindow::ComponentWindow(int x, int y, int width, int height, const char* _name) {
    win = newwin(height, width, y, x);
    name = _name;
}
