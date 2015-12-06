//
// Created by pawkrol on 12/1/15.
//

#ifndef TCPSERVER_SOCKETCONNECTOR_H
#define TCPSERVER_SOCKETCONNECTOR_H

#include <iostream>
#include <netdb.h>
#include "Packet.h"

class SocketConnector {
private:
    addrinfo *hostList;
    int sockfd;
    int clientfd;
    std::string port;

    socklen_t sckSize;
    sockaddr_storage clientAddr;

    void setStructs();
    void setSocket();
    void makeBind();
    void freeHostsList();
    void checkStatus();

public:
    int status;

    SocketConnector(std::string port);
    ~SocketConnector();

    void makeListen();
    void makeAccept();
    ssize_t sendMessage(std::string msg);
    ssize_t sendMessage(Packet *packet);
    ssize_t getResponse(void *buff, size_t size);
    ssize_t getResponse(Packet *&packet);

    void closeClientSocket();

    std::string getReadableClientAddr();
};


#endif //TCPSERVER_SOCKETCONNECTOR_H
