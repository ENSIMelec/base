#ifndef ACTIONMANAGER_H_INCLUDED
#define ACTIONMANAGER_H_INCLUDED

#include <iostream>
#include <string>
#include <thread>
#include <sys/time.h>
#include <vector>
#include "ActionServo.h"
#include "AX12Manager.h"
//#include "ClientUDP.h"
#include "FichierAction.h"
#include "ServoManager.h"
//#include "StepperManager.h"

using namespace std;

class ActionManager
{

public:
	ActionManager(int i2c_Servos, int nbAX12, const string &actionsPath);
	void close();
	void action(const std::string& filename);

private:
	void sleepMillis(int millis);
	//ClientUDP &client;
	ServoManager servoManager;
	//StepperManager stepper;
	AX12Manager ax12Manager;
    string path;
};

#endif // ACTIONMANAGER_H_INCLUDED