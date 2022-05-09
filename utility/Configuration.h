//
// Created by Tom on 07/05/2022.
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <iostream>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

using namespace std;

class Configuration {

private:
    ~Configuration();
    // Arbre de configuration
    boost::property_tree::ptree tree;

public:
    static Configuration& instance(){
        // Instanciation au premier appel seulement
        static Configuration config("/home/pi/Documents/Krabbs/res/config.info");
        return config;
    }

    double getDouble(std::string string);
    int getInt(std::string string);
    float getFloat(std::string string);

    explicit Configuration(string filename);
};

#endif //CONFIGURATION_H
