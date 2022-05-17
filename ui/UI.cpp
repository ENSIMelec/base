//
// Created by Tom on 15/05/2022.
//

#include "UI.h"
#include "OdometryWindow.h"
#include "ControllerWindow.h"
#include "StrategyWindow.h"
#include "SerialCoderManagerWindow.h"
#include "ActionManagerWindow.h"
#include "InformationWindow.h"
#include "ConsoleWindow.h"

void UI::init() {

    setlocale(LC_CTYPE, "");

    initscr();
    timeout(0);

    noecho();
    curs_set(0);

    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);

    attron(COLOR_PAIR(1));
    printw("Kiroulpa :");
    attroff(COLOR_PAIR(1));

    refresh();

    // Getting the size of the current consoleWin window
    getmaxyx(stdscr, height, width);

    components.push_back(new ConsoleWindow(0, height / 2, width, height / 2));

    // Left column
    components.push_back(new OdometryWindow(0, 1, width / 3, 5));
    components.push_back(new SerialCoderManagerWindow(0, 6, width / 3, 3));
    components.push_back(new StrategyWindow(0, 9, width / 3, 6));

    // Middle column
    components.push_back(new ControllerWindow(width/3, 1, width / 3, 9));

//    components.push_back(new ActionManagerWindow(2 * width / 3, 5, width / 3, 5));
//    components.push_back(new InformationWindow(width / 3, 10, 2 * width / 3, 5));
}

void UI::display() {

    for(ComponentWindow* c : components) {
        WINDOW * win = c->getWin();
        box(win, 0, 0);

        // Display component title
        mvwprintw(win, 0, 2, "%s", c->getName());
        c->display();

        // Refresh the window
        wrefresh(win);
    }

    // Refresh the whole console
    refresh();
}