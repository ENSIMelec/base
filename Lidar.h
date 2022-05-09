//#ifndef LIDAR_H_
//#define LIDAR_H_

#include <wiringSerial.h>
#include <wiringPi.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <ctime>
#include <thread>


// A décommenter si vous voulez que le debug du lidar soit actif. Le debug va écrire dans la console à chaque évenement jugé important mais non gênant.
#define LIDAR_DEBUG




#define SERIAL_PORT "/dev/serial0"	//Port série interne de la raspPi, relié aux pins 8 (Tx) et 10 (Rx)
#define PWM_PIN_LIDAR 1  		// Ne pas modifier !!! C'est la seule borne PWM de la rasp PI !
#define DEFAULT_LIDAR_SPEED 512
#define DEFAULT_LIDAR_RUN_TIME 95

#define ANGULAR_PRECISION 6
#define ANGLE_MAX 23040
#define DISTANCE_MAX 65535
#define DISTANCE_MIN 50
#define DISTANCE_WARNING 700
#define DISTANCE_DANGER 400
#define WARNING_QUOTA 5
#define DANGER_QUOTA 5
#define DANGER_TIME 100
#define TTL_LOCAL 100


const char SCAN[] = {0xA5,0x20};
const char STOP[] = {0xA5,0x25};
const char RESET[] = {0xA5,0x40};


class Lidar{
	
	private:
		
		static const uint8_t SCAN_HEADER[7];

		// Compteur pour quand un obstacle est proche (ie : le robot d'en face) 
		inline static int dangerCounter, warningCounter;

		// Etat de la liaison série avec le lidar.
		inline static int serialState, serialID;

		inline static bool isRunning;

        static void setMotorSpeed(int speed);
        static void sendCommand(const char command[]);

		/*
		// Grille du Lidar une fois le traitement effectué. La grille représente la table de jeu et la position des "murs"
		inline static int gridX, gridY, ***grid;
		*/
	public:
		// Commandes du lidar. A envoyer via "serialPuts"
		static const char SCAN[2];
		static const char STOP[2];
		static const char RESET[2];

		static void println(const char* msg);

		static void init();
		static void start(int speed = DEFAULT_LIDAR_SPEED);
		static void stop();
		static bool test();
		static void run(int runtime = DEFAULT_LIDAR_RUN_TIME, int motorSpeed = DEFAULT_LIDAR_SPEED);

        static bool isActive();
		static bool isDanger();
		static bool isWarning();
};






//#endif // LIDAR_H_
