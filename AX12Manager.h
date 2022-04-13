#ifndef AX12MANAGER_H_INCLUDED
#define AX12MANAGER_H_INCLUDED


#include <iostream>
#include <stdlib.h> //The function abs() is in the example code, and it needs stdlib.h to be included.
#include <stdio.h> //The example shows Dynamixel status in sequence by the function printf(). So here stdio.h is needed.


#include <dynamixel_sdk/dynamixel_sdk.h>
using namespace dynamixel;

#include "dynamixel_sdk/port_handler_linux.h"
#include "dynamixel_sdk/protocol1_packet_handler.h"

// Control table address
#define ADDR_AX_BAUD_RATE                  4
#define ADDR_AX_TORQUE_ENABLE           24                  // Control table address is different in Dynamixel model
#define ADDR_AX_GOAL_POSITION           30
#define ADDR_AX_PRESENT_POSITION        36
#define ADDR_AX_PRESENT_VOLTAGE         42
#define ADDR_AX_SPEED_VALUE             32
#define ADDR_AX_TORQUE_LIMIT            34
#define ADDR_AX_PRESENT_LOAD            40

#define READ_DATA   0x02
#define HEADER      0xFF
//Dynamixel series have their own control tables: Addresses and Byte Length in each items. To control one of the items, its address (and length if necessary) is required. Find your requirements in http://support.robotis.com/.

// Protocol version
#define PROTOCOL_VERSION                1.0                 // See which protocol version is used in the Dynamixel
// Default setting
#define BAUDRATE                        1000000
#define DEVICENAME                      "/dev/ttyACM0"      // Check which port is being used on your controller
#define TORQUE_ENABLE 1
#define TORQUE_DISABLE 0

/*
COMM_TX_FAIL
COMM_SUCCESS
*/

class AX12Manager
{

public:
	AX12Manager(int nbAX);
	void close();
	int AX12Action(int numActionneur, int angleAction, int forceAction);

private:
	int initialisation();
	int nbAX12;
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler;
};

#endif //AX12MANAGER_H_INCLUDED