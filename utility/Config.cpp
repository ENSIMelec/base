#include "Config.h"

using namespace std;
namespace pt = boost::property_tree;

Config::Config(const string& filename) {
    loadFromFile(filename);
}

void Config::loadFromFile(const string& filename)
{
	// Creating ptree object
	pt::ptree tree;

	// Loading the file using property_tree
    try {
        pt::read_info(filename, tree);
    } catch (const pt::ptree_error &error) {
        cout << error.what() << endl;
    }

    // Reading the attributes from the config file
    distanceSpeedMax = tree.get<float>("ramp.distance_speed_max");
    distanceAccelerationMax = tree.get<float>("ramp.distance_acceleration_max");
    distanceDecelerationMax = tree.get<float>("ramp.distance_deceleration_max");
    distanceAnticipationGain = tree.get<float>("ramp.distance_anticipation_gain");
    distanceWindow = tree.get<float>("ramp.distance_window");
    angleSpeedMax = tree.get<float>("ramp.angle_speed_max");
    angleAccelerationMax = tree.get<float>("ramp.angle_acceleration_max");
    angleDecelerationMax = tree.get<float>("ramp.angle_deceleration_max");
    angleAnticipationGain = tree.get<float>("ramp.angle_anticipation_gain");


    diametreRoue = tree.get<float>("odometry.diametre_roue");
    resolution = tree.get<float>("odometry.resolution");
    coeffCorrecteur = tree.get<float>("odometry.coeff_correcteur");
    entraxe = tree.get<float>("odometry.entraxe");

    coeffAcceleration = tree.get<double>("asservissement.coeff_acceleration");
	pid_kpA = tree.get<double>("asservissement.pid_kpa");
	pid_kiA = tree.get<double>("asservissement.pid_kia");
	pid_kdA = tree.get<double>("asservissement.pid_kda");

	pid_kpdepPathFinding = tree.get<double>("asservissement.pid_kpdepPathFinding");
	pid_kpDep = tree.get<double>("asservissement.pid_kpdep");
	pid_kiDep = tree.get<double>("asservissement.pid_kidep");
	pid_kdDep = tree.get<double>("asservissement.pid_kddep");

	pid_kpPos = tree.get<double>("asservissement.pid_kppos");
	pid_kiPos = tree.get<double>("asservissement.pid_kipos");
	pid_kdPos = tree.get<double>("asservissement.pid_kdpos");

	pid_kpV_low = tree.get<double>("asservissement.pid_kpV_low");
	pid_kiV_low = tree.get<double>("asservissement.pid_kiV_low");
	pid_kdV_low  = tree.get<double>("asservissement.pid_kdV_low");

	pid_kpV_medium = tree.get<double>("asservissement.pid_kpV_medium");
	pid_kiV_medium = tree.get<double>("asservissement.pid_kiV_medium");
	pid_kdV_medium  = tree.get<double>("asservissement.pid_kdV_medium");

	pid_kpV_high = tree.get<double>("asservissement.pid_kpV_high");
	pid_kiV_high = tree.get<double>("asservissement.pid_kiV_high");
	pid_kdV_high  = tree.get<double>("asservissement.pid_kdV_high");

	CoeffGLong = tree.get<double>("asservissement.CoeffGLong");
	CoeffDLong = tree.get<double>("asservissement.CoeffDLong");
	CoeffAnglD = tree.get<double>("asservissement.CoeffAnglD");
	CoeffAnglG = tree.get<double>("asservissement.CoeffAnglG");

	I2C_SERVOS = tree.get<int>("i2c.servos");
    I2C_MOTORS = tree.get<int>("i2c.motors");

    match_time = tree.get<int>("global.match_time");
    nbAX12 = tree.get<int>("global.nbAX12");
    update_time = tree.get<int>("global.update_time");

#ifdef DEBUG_CONFIG
    // Display the config
	printConfig();
#endif
}
int Config::getUpdateTime() const { return update_time; }

int Config::getNbAX12() const { return nbAX12; }

double Config::getPIDkpA() const { return pid_kpA; }
double Config::getPIDkiA() const { return pid_kiA; }

double Config::getPIDkdA() const { return pid_kdA; }
double Config::getPIDkpDep() const { return pid_kpDep; }
double Config::getPIDkiDep() const { return pid_kiDep; }

double Config::getPIDkdDep() const { return pid_kdDep; }
double Config::getPIDkpPos() const { return pid_kpPos; }
double Config::getPIDkiPos() const { return pid_kiPos; }

double Config::getPIDkdPos() const { return pid_kdPos; }
double Config::getPIDkpVLow() const { return pid_kpV_low; }
double Config::getPIDkiVLow() const { return pid_kiV_low; }

double Config::getPIDkdVLow() const { return pid_kdV_low; }
double Config::getPIDkpVMedium() const { return pid_kpV_medium; }
double Config::getPIDkiVMedium() const { return pid_kiV_medium; }

double Config::getPIDkdVMedium() const { return pid_kdV_medium; }
double Config::getPIDkpVHigh() const { return pid_kpV_high; }
double Config::getPIDkiVHigh() const { return pid_kiV_high; }

double Config::getPIDkdVHigh() const { return pid_kdV_high; }
double Config::getCoeffGLong() const { return CoeffGLong; }
double Config::getCoeffDLong() const { return CoeffDLong; }
double Config::getCoeffAnglD() const { return CoeffAnglD; }

double Config::getPIDkpDepPathfinding() const {return pid_kpdepPathFinding;}

double Config::getCoeffAnglG() const { return CoeffAnglG; }
int Config::get_I2C_SERVOS() const { return I2C_SERVOS; }
int Config::get_I2C_MOTORS() const { return I2C_MOTORS; }

int Config::getMatchTime() const { return match_time; }

void Config::printConfig() const {
	cout << endl << "--- CONFIGURATION ---" << endl;
	cout << "Temps du match : " << getMatchTime() << endl;
    cout << "Temps minimum entre deux appels d'asservissement : " << getUpdateTime() << endl << endl;
	cout << "Nombre d'ax12Manager à connecter : " << getNbAX12() << endl;

	cout << "Coefficients : " << endl;
	cout << "-Longueur :" << endl << "\tGauche : " << getCoeffGLong() << endl << "\tDroite : " << getCoeffDLong() << endl;
	cout << "-Angle Droite: "<< getCoeffAnglD()<< "Angle Gauche" << getCoeffAnglG() << endl;

	cout << "PID : " << endl;
	cout << "-Vitesse faible:" << endl << "\tP = " << getPIDkpVLow() << endl << "\tI = " << getPIDkiVLow() << endl << "\tD = " << getPIDkdVLow() << endl;
	cout << "-Vitesse moyenne:" << endl << "\tP = " << getPIDkpVMedium() << endl << "\tI = " << getPIDkiVMedium() << endl << "\tD = " << getPIDkdVMedium() << endl;
	cout << "-Vitesse haute:" << endl << "\tP = " << getPIDkpVHigh() << endl << "\tI = " << getPIDkiVHigh() << endl << "\tD = " << getPIDkdVHigh() << endl;
	cout << "-Angle :" << endl << "\tP = " << getPIDkpA() << endl << "\tI = " << getPIDkiA() << endl << "\tD = " << getPIDkdA() << endl;
	cout << "-Deplacement :" << endl << "\tP = " << getPIDkpDep() << endl << "\tI = " << getPIDkiDep() << endl << "\tD = " << getPIDkdDep() << endl;
	cout << "-Position :" << endl << "\tP = " << getPIDkpPos() << endl << "\tI = " << getPIDkiPos() << endl << "\tD = " << getPIDkdPos() << endl;

	cout << "I2C :" << endl;
	cout << "\tSERVOS : " << get_I2C_SERVOS() << endl;
	cout << "\tMOTEURS : " << get_I2C_MOTORS() << endl;
	cout << "--- FIN DE LA CONFIGURATION ---" << endl;
}

float Config::getDistanceDecelerationMax() {
    return distanceDecelerationMax;
}

float Config::getDistanceAccelerationMax() {
    return distanceAccelerationMax;
}

float Config::getDistanceSpeedMax() {
    return distanceSpeedMax;
}

float Config::getDistanceAnticipationGain() {
    return distanceAnticipationGain;
}

float Config::getDistanceWindow() {
    return distanceWindow;
}

float Config::getAngleAnticipationGain() {
    return angleAnticipationGain;
}

float Config::getAngleDecelerationMax() {
    return angleDecelerationMax;
}

float Config::getAngleAccelerationMax() {
    return angleAccelerationMax;
}

float Config::getAngleSpeedMax() {
    return angleSpeedMax;
}

float Config::getEntraxe() {
    return entraxe;
}

float Config::getCoeffCorrecteur() {
    return coeffCorrecteur;
}

float Config::getResolution() {
    return resolution;
}

float Config::getDiametreRoue() {
    return diametreRoue;
}

double Config::getCoeffAcceleration() {
    return coeffAcceleration;
}
