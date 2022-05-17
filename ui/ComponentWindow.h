//
// Created by Tom on 15/05/2022.
//

#ifndef KONSOLE_COMPONENT_WINDOW_H
#define KONSOLE_COMPONENT_WINDOW_H

#include <curses.h>
#include <string>

class ComponentWindow {

public:
    ComponentWindow(int x, int y, int width, int height, const char* name);
    virtual void display() = 0;

    [[nodiscard]] WINDOW* getWin() const {return win;}
    [[nodiscard]] const char* getName() const {return name;}
private:
    const char* name;
protected:
    WINDOW* win;
};


#endif //KONSOLE_COMPONENT_WINDOW_H
