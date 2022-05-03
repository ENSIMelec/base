//
// Created by Tom on 29/04/2022.
//

#include "StrategyParser.h"

#include <iostream>
#include <fstream>

#include "lib/json.hpp"
#include "MathUtils.h"

using json = nlohmann::json;

/**
 * Reads the strategy from a file and create the corresponding objects
 * @param path The path to the strategy directory
 * @param strategyFileName The strategy file you want to parse
 * @return status code
 */
vector<struct Objective *> * StrategyParser::readObjectives(const string& path, const string& strategyFileName) {

    auto * objectives = new vector<Objective *>();

    ifstream strategyFile;
    strategyFile.open(path + strategyFileName);
    if(strategyFile.fail()) {
        cout << strategyFileName << " was not successfully opened.\nCheck that the file currently exists." << endl;
        StrategyParser::status = FILE_OPENING_ERROR;
    } else {
        cout << "Successfully opened " << strategyFileName << endl;
    }

    string line;
    while(getline(strategyFile, line)) {
        // Note : npos is the return value if the char has not been found

        if(line.find('(', 0) != string::npos) {
            string obj = line.substr(0,line.size() - 1);
            //Si le caractere est trouve, on est dans une situation de "choix". il faudra parser la ligne pour la traiter.
            //On traitera la ligne a la volee dans le programme, on l'enregistre juste
//            cout << "Choix : " << obj << endl;

            StrategyParser::status = parseObjectivePoints(path, obj);
            objectives->push_back(StrategyParser::currentObjective);
        }

        // Add objective stored into a JSON file
        else if(line.find(".json") != string::npos) {
            string obj = line.substr(0,line.find(".json") + 5);
//            cout << "Ligne : " << obj <<endl;

            StrategyParser::status = parseObjectivePoints(path, obj);
            objectives->push_back(StrategyParser::currentObjective);
        }

        // Create a new objective
        else if(line.find('[') != string::npos) {
            string objName = line.substr(1, line.find(']') - 1);
            StrategyParser::currentObjective = new Objective(objName);

            if(line.find("@facultatif") != string::npos){
                StrategyParser::currentObjective->setOptional(true);
            }

//            cout << "[" << objName << "] " << endl;
        }

        if(StrategyParser::status == FILE_OPENING_ERROR) return objectives;
    }

    strategyFile.close();
    StrategyParser::status = EXIT_SUCCESS;
    return objectives;
}

int StrategyParser::parseObjectivePoints(const string& path, const string& objFileName) {
    // Opening the Json file
    ifstream jsonFile(path + objFileName);
    if(jsonFile.fail()) {
        cout << objFileName << " was not successfully opened.\nCheck that the file currently exists." << endl;
        return FILE_OPENING_ERROR;
    }

    // Parsing the file to a Json object
    json jsonObj;
    jsonFile >> jsonObj;

    // Reading the Json object
    for (auto& [key, value] : jsonObj.items()) {
        if(key == "Points") {
            json j_points = value;
            for (auto& [key, value] : j_points.items()) {
                auto * point = new Point();
                StrategyParser::currentObjective->addPoint(point);

                json j_point = value;
                for (auto& [key, value] : j_point.items()) {

//                    cout << key << ": " << value << endl;

                    if(key == "X") {
                        point->setX(value);
                    } else if(key == "Y") {
                        point->setY(value);
                    } else if(key == "angle") {
                        point->setTheta(MathUtils::deg2rad(value));
                    } else if(key == "action") {
                        point->setAction(value);
                    } else if(key == "attAction") {
                        point->setActionWaiting(true);
                    } else if(key == "blocage") {
                        point->setBlocage(value);
                    } else if(key == "commentaire") {
                        point->setCommentary(value);
                    } else if(key == "type") {
                        point->setType(value);
                    } else if(key == "vitesse") {
                        point->setSpeed(value);
                    } else if(key == "sens") {
                        point->setDirection(value);
                    }
                }
            }
        }
    }

    jsonFile.close();
    return EXIT_SUCCESS;
}
