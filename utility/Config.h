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

    explicit Config(const string& filename);

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
	int getUpdateTime() const;
	int getNbAX12() const;

	double getPIDkpA() const;
	double getPIDkiA() const;
	double getPIDkdA() const;

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
	int get_I2C_MOTORS() const;

	int getMatchTime() const;

    double getPIDkpDepPathfinding() const;

    float getDistanceSpeedMax();

    float getDistanceAccelerationMax();

    float getDistanceDecelerationMax();

    float getDistanceAnticipationGain();

    float getDistanceWindow();

    float getAngleSpeedMax();

    float getAngleAccelerationMax();

    float getAngleDecelerationMax();

    float getAngleAnticipationGain();

    float getDiametreRoue();

    float getResolution();

    float getCoeffCorrecteur();

    float getEntraxe();

private:
	// Attributes
	int update_time{};
	int nbAX12{};

	double pid_kpdepPathFinding{}, pid_kpDep{}, pid_kiDep{}, pid_kdDep{};
	double pid_kiPos{},pid_kpPos{}, pid_kdPos{};
	double pid_kpA{}, pid_kiA{}, pid_kdA{};
	double pid_kpV_low{}, pid_kiV_low{}, pid_kdV_low{}, pid_kpV_medium{}, pid_kiV_medium{}, pid_kdV_medium{}, pid_kpV_high{}, pid_kiV_high{}, pid_kdV_high{};

	double CoeffGLong{}, CoeffDLong{}, CoeffAnglD{}, CoeffAnglG{};

	int I2C_SERVOS{}, I2C_MOTORS{};

	int match_time{};

	bool WAIT_DEVICES_CONNECTIONS{};

    float distanceSpeedMax;
    float distanceAccelerationMax;
    float distanceDecelerationMax;
    float distanceAnticipationGain;
    float distanceWindow;
    float angleSpeedMax;
    float angleAccelerationMax;
    float angleDecelerationMax;
    float angleAnticipationGain;

    // Odometry
    float diametreRoue;
    float resolution;
    float coeffCorrecteur;
    float entraxe;
};

#endif //CONFIG_H_INCLUDED