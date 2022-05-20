//
// Created by Tom on 29/04/2022.
//

#include "StrategyParser.h"
#include "../lib/json.hpp"

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
        cout << strategyFileName<< " was not successfully opened.\nCheck that the file currently exists." << endl;
        StrategyParser::status = FILE_OPENING_ERROR;
    } else {
//        cout << "Successfully opened " << strategyFileName << endl;
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
            auto * objName = new string(line.substr(1, line.find(']') - 1));
            StrategyParser::currentObjective = new Objective(objName->c_str());

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
                    if (key == "X") {
                        point->setX(value);
                    } else if (key == "Y") {
                        point->setY(value);
                    } else if (key == "angle") {
                        point->setTheta(MathUtils::deg2rad(value));
                    } else if (key == "action") {
                        point->setAction(value);
                    } else if(key == "time") {
                        point->setWaitingTime(value);
                    }else if(key == "attAction") {
                        point->setActionAfterMovement(true);
                    } else if(key == "distance_threshold") {
                        point->setDistanceThreshold(value);
                    } else if(key == "type") {
                        if(value == "StaticPosition") {
                            point->setType(PointType::STATIC_POSITION);
                        } else if(value == "RelativeAngle") {
                            point->setType(PointType::RELATIVE_ANGLE);
                        } else if(value == "AbsoluteAngle") {
                            point->setType(PointType::ABSOLUTE_ANGLE);
                        } else if(value == "MoveToPosition") {
                            point->setType(PointType::MOVE_TO_POSITION);
                        } else if(value == "MoveToPositionRelativeToAngle") {
                            point->setType(PointType::MOVE_TO_POSITION_RELATIVE_TO_ANGLE);
                        } else if(value == "SetX") {
                            point->setType(PointType::SET_X);
                        } else if(value == "SetY") {
                            point->setType(PointType::SET_Y);
                        } else if(value == "SetXYTheta") {
                            point->setType(PointType::SET_XY_THETA);
                        } else if(value == "Wait") {
                            point->setType(PointType::WAIT);
                        } else if(value == "Action") {
                            point->setType(PointType::ACTION);
                        } else if(value == "Angle") {
                            point->setType(PointType::ANGLE);
                        }
                    } else if(key == "direction") {
                        if(value == "backward") point->setDirection(MovementController::Direction::BACKWARD);
                    } else if(key == "max_speed") {
                        point->setMaxSpeed(value);
                    } else if(key == "timeout") {
                        point->setTimeout(value);
                    }
                }
            }
        }
    }

    jsonFile.close();
    return EXIT_SUCCESS;
}
