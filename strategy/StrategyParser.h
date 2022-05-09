//
// Created by Tom on 29/04/2022.
//

#ifndef KRABBS_STRATEGY_PARSER_H
#define KRABBS_STRATEGY_PARSER_H

// Status
#define EXIT_SUCCESS        0
#define FILE_OPENING_ERROR  1

// Includes
#include <string>
#include <vector>
#include "Objective.h"

using namespace std;

class StrategyParser {
public:
    static vector<Objective*>* readObjectives(const string& path, const string& strategyFileName);

    /**
     * Check the status of the Strategy Parser for errors
     * @return EXIT_SUCCESS or FILE_OPENING_ERROR
     */
    static int getStatus() {return status;}
private:
    inline static int status;
    inline static Objective * currentObjective;

    static int parseObjectivePoints(const string& path, const string& objFileName);
};


#endif //KRABBS_STRATEGY_PARSER_H
