//
// Created by cleme on 12/05/2022.
//

#ifndef KIROULPA_SERVERUDP_H
#define KIROULPA_SERVERUDP_H


#include "ServerUDP.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>

using namespace std;


class ServerUDP
{
public:
    ServerUDP(const std::string& addr, int port);
    void PrintCmd(char* string);

private:


};

#endif //KIROULPA_SERVERUDP_H
