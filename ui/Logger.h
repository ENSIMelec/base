//
// Created by Tom on 15/05/2022.
//

#ifndef KONSOLE_CONSOLE_WINDOW_H
#define KONSOLE_CONSOLE_WINDOW_H

#include <curses.h>
#include <string>
#include <vector>
#include "ComponentWindow.h"

using namespace std;

class Logger : public ComponentWindow {
public:
    Logger(int x, int y, int width, int height) : ComponentWindow(x, y, width, height, "Logger") {
        // We remove two lines for the top and bottom borders
        logHeight = height - 2;
    };

    void println(string * s);
    void display() override;

    void goUp();

    void goDown();

private:
    int position = 0;
    int logHeight = 0;
    vector<string *> logs;
};


#endif //KONSOLE_CONSOLE_WINDOW_H
