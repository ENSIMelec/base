//
// Created by Taoufik on 27/03/2020.
//

#include "Configuration.h"

namespace pt = boost::property_tree;

Configuration::Configuration(string filename) {
    // Chargement depuis un fichier au format INFO
    pt::read_info(filename, this->tree);
}
double Configuration::getDouble(std::string string) {

    // Lecture des attributs depuis le fichier de configuration
    std::stringstream ss;
    ss << string;

    return tree.get<double>(ss.str());
}
float Configuration::getFloat(std::string string) {
    std::stringstream ss;
    ss << string;

    return tree.get<float>(ss.str());
}
int Configuration::getInt(std::string string) {
    std::stringstream ss;
    ss << string;

    return tree.get<int>(ss.str());
}
Configuration::~Configuration() {}
