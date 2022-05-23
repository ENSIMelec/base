//
// Created by cleme on 22/05/2022.
//

#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<thread>
#include <iostream>

#include "ServerUDP.h"

#define PORT 4445
void *connection_handler(void *);


ServerUDP::ServerUDP(const std::string &addr, int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }else
        perror("socket created");

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }else
        perror("sock attached");
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }else
        perror("bind success");
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }else
        perror("listen success");

    while(new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))
    {
        perror("accept");
        //cout << "putain wesh" << endl;

        if(read( new_socket , buffer, 1024-1) < 0 ){
            perror("no message found");
        }else{
            cout << "message reçu " << endl;
            printf("%s\n",buffer);

            send(new_socket , hello , strlen(hello) , 0 );
            printf("Hello message sent\n");
        }

    }
    return;
}

void ServerUDP::PrintCmd(char* string){
    cout << "Nous sommes ici chacal : " << string << endl;
}


