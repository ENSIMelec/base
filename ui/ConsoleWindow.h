//
// Created by Tom on 15/05/2022.
//

#ifndef KONSOLE_CONSOLE_WINDOW_H
#define KONSOLE_CONSOLE_WINDOW_H

#include <curses.h>
#include <string>
#include <vector>
#include "ComponentWindow.h"
#include "UI.h"

using namespace std;

class ConsoleWindow : public ComponentWindow {
public:
    ConsoleWindow(int x, int y, int width, int height) : ComponentWindow(x, y, width, height, "Console") {
        UI::console = win;
    };

    void print(const string& s);
    void println(const string& s);
    void printDouble(double d);

    void display() override;
private:
    vector<string*> buffer;

    void pushLine(string * line);
};


#endif //KONSOLE_CONSOLE_WINDOW_H
