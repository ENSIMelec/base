//
// Created by Tom on 15/05/2022.
//

#include "Logger.h"

void Logger::println(string * s) {
    logs.push_back(s);
}

void Logger::display() {

    int nbOfLogs = (int) logs.size();

    if(nbOfLogs <= logHeight) {
        for(int logIndex=0; logIndex < nbOfLogs; logIndex++) {
            const char * s = logs[logIndex]->c_str();
            mvwprintw(win, logIndex + 1, 1, "%s\n", s);
        }
    } else {
        int i = 0;
        for(int lineIndex = logHeight; lineIndex > 0; lineIndex--) {
            int logIndex = nbOfLogs - i - 1 - position;
            i++;

            const char * s = logs[logIndex]->c_str();
            mvwprintw(win, lineIndex, 1, "%s\n", s);
        }
    }
}

void Logger::goDown() {
    if(position > 0) position--;
}

void Logger::goUp() {
    if(position < (logs.size() - logHeight)) position++;
}
