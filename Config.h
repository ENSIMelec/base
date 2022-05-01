#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

using namespace std;

class Config
{
public:

    explicit Config(const string& basicString);

/**
	 * @brief Load the configuration from a file
	 * @param filename Path to the config file (.info)
	 */
	void loadFromFile(const std::string& filename);

    /// @brief Print the configuration to the console
	void printConfig() const;

    /**
     * @brief Get the waiting time between each call to the asserv call
     * @return Delta time in milliseconds
     */
	int getDeltaAsserv() const;
	int getNbAX12() const;

	std::string getIpServer() const;
	int getPort() const;

	double getPIDkpA() const;
	double getPIDkiA() const;
	double getPIDkdA() const;

	double getPIDkpDepPathfinding() const;
	double getPIDkpDep() const;
	double getPIDkiDep() const;
	double getPIDkdDep() const;

	double getPIDkpPos() const;
	double getPIDkiPos() const;
	double getPIDkdPos() const;

	double getPIDkpVLow() const;
	double getPIDkiVLow() const;
	double getPIDkdVLow() const;

	double getPIDkpVMedium() const;
	double getPIDkiVMedium() const;
	double getPIDkdVMedium() const;

	double getPIDkpVHigh() const;
	double getPIDkiVHigh() const;
	double getPIDkdVHigh() const;

	double getCoeffGLong() const;
	double getCoeffDLong() const;
	double getCoeffAnglD() const;
	double getCoeffAnglG() const;

	int get_I2C_SERVOS() const;
	int get_I2C_LANCEUR() const;
	int get_I2C_MOTEURS() const;
	int get_I2C_STEPPER() const;

	int get_temps_match() const;

	bool get_WAIT_DEVICES_CONNECTIONS() const;

private:
	// Attributes
	int delta_asserv{};
	int nbAX12{};

	std::string ipServeur;
	int port{};

	double pid_kpdepPathFinding{}, pid_kpDep{}, pid_kiDep{}, pid_kdDep{};
	double pid_kiPos{},pid_kpPos{}, pid_kdPos{};
	double pid_kpA{}, pid_kiA{}, pid_kdA{};
	double pid_kpV_low{}, pid_kiV_low{}, pid_kdV_low{}, pid_kpV_medium{}, pid_kiV_medium{}, pid_kdV_medium{}, pid_kpV_high{}, pid_kiV_high{}, pid_kdV_high{};

	double CoeffGLong{}, CoeffDLong{}, CoeffAnglD{}, CoeffAnglG{};

	int I2C_SERVOS{}, I2C_LANCEUR{}, I2C_MOTEURS{},I2C_STEPPER{};

	int temps_match{};

	bool WAIT_DEVICES_CONNECTIONS{};
};

#endif //CONFIG_H_INCLUDED