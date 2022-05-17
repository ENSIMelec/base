//
// Created by Tom on 15/05/2022.
//

#ifndef UI_UI
#define UI_UI


#include <curses.h>
#include <vector>
#include "ComponentWindow.h"

using namespace std;

class UI {
public:
    static void init();
    static void display();

    static int getWidth() {return width;}
    static int getHeight() {return height;}

    static inline WINDOW* console;
private:
    static inline int width, height;

    static inline vector<ComponentWindow*> components;
};

#endif // UI_UI
