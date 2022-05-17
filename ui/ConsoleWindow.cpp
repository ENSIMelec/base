//
// Created by Tom on 15/05/2022.
//

#include "ConsoleWindow.h"
#include "UI.h"

#define MAX_SIZE 10

void ConsoleWindow::print(const string& s) {
    auto * s_ = new string();
    for(char c : s) {
        if(c != '\n') {
            s_ += c;
        } else {
            pushLine(s_);
            s_->clear();
        }
    }

    display();
}

void ConsoleWindow::println(const string& s) {
    auto * s_ = new string();
    for(char c : s) {
        if(c != '\n') {
            s_ += c;
        } else {
            pushLine(s_);
            s_->clear();
        }
    }

    pushLine(s_);

    display();
}

void ConsoleWindow::printDouble(double d) {
    const char *s = to_string(d).c_str();
    mvprintw(0, 0, "%s", s);
}

void ConsoleWindow::pushLine(string * line) {
    buffer.push_back(line);
}

void ConsoleWindow::display() {

}
