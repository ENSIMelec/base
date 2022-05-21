//
// Created by Tom on 15/05/2022.
//

#include "UI.h"
#include "OdometryWindow.h"
#include "ControllerWindow.h"
#include "StrategyWindow.h"
#include "SerialCoderManagerWindow.h"

void UI::init() {

    setlocale(LC_CTYPE, "");

    initscr();
    timeout(0);

    noecho();
    keypad(stdscr, true);
    cbreak();
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

    // The logger does not depend on any module
    logger = new Logger(0, height / 2, width, height / 2);
    components.push_back(logger);
}

void UI::update() {
    int c = getch();
    if(c == KEY_UP) {
        logger->goUp();
    } else if(c == KEY_DOWN) {
        logger->goDown();
    } else if(c == 'q') {
        shouldQuit = true;
    }
}

/** This function must be called after each module has been initialized */
void UI::initModules() {

    // Left column
    components.push_back(new OdometryWindow(0, 1, width / 3, 5));
    components.push_back(new SerialCoderManagerWindow(0, 6, width / 3, 3));
//    components.push_back(new StrategyWindow(0, 9, width / 3, 6));

    // Middle column
//    components.push_back(new ControllerWindow(width/3, 1, width / 3, 9));

//    components.push_back(new ActionManagerWindow(2 * width / 3, 5, width / 3, 5));
//    components.push_back(new InformationWindow(width / 3, 10, 2 * width / 3, 5));
}

void UI::logAndRefresh(const char *s) {
    WINDOW * win = logger->getWin();
    box(win, 0, 0);
    mvwprintw(win, 0, 2, logger->getName());

    logger->println(new string(s));

    logger->display();
    wrefresh(win);
    refresh();
}

void UI::log(const char *s) {
    logger->println(new string(s));
}

void UI::display() {

    for(ComponentWindow* c : components) {
        WINDOW * win = c->getWin();

        // Display component title
        mvwprintw(win, 0, 2, "%s", c->getName());
        c->display();

        box(win, 0, 0);

        // Refresh the window
        wrefresh(win);
    }

    // Refresh the whole console
    refresh();
}

void UI::end() {
    display();

    curs_set(0);
    attron(COLOR_PAIR(1));
    printw(" press q to exit ...");
    attroff(COLOR_PAIR(1));

    timeout(10000);

    delay(2000);

    refresh();
    while(UI::shouldQuit) {
        UI::update();
        UI::display();
    }

    endwin();
}
