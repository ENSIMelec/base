//
// Created by priskn on 07/05/22.
//


#ifndef UDP_UDPROBOT_H
#define UDP_UDPROBOT_H

#include "ServerUDP.h"


class UDPRobot {
    public:
        UDPRobot();

    private:
        ServerUDP serveur;

    void gestion(char *chaine);
};


#endif //UDP_UDPROBOT_H
