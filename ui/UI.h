//
// Created by Tom on 15/05/2022.
//

#ifndef UI_UI
#define UI_UI


#include <curses.h>
#include <vector>
#include <filesystem>
#include "ComponentWindow.h"
#include "Logger.h"

using namespace std;

class UI {
public:
    static void init();

    static void update();
    static void display();

    static void end();

    static int getWidth() {return width;}
    static int getHeight() {return height;}

    static void initModules();
    static void logAndRefresh(const char *s);
    static void log(const char *s);

private:
    static inline int width, height;

    static inline Logger *logger;
    static inline vector<ComponentWindow*> components;
    static inline bool shouldQuit = false;
};

#endif // UI_UI
