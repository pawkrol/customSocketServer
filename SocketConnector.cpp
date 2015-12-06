//
// Created by pawkrol on 12/1/15.
//

#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#include "SocketConnector.h"

SocketConnector::SocketConnector(std::string port) {
    this->port = port;

    setStructs();
    checkStatus();

    setSocket();
    makeBind();

    freeHostsList();
}

SocketConnector::~SocketConnector() {
    close(sockfd);
}

void SocketConnector::checkStatus() {
    if (status != 0){
        exit(-1);
    }
}

void SocketConnector::setStructs() {
    addrinfo host;
    addrinfo *hostList;

    memset(&host, 0, sizeof(host));

    host.ai_family = AF_UNSPEC;
    host.ai_socktype = SOCK_STREAM;
    host.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, port.c_str(), &host, &hostList);

    this->hostList = hostList;
}

void SocketConnector::setSocket() {
    int yes = 1;
    sockfd = socket(hostList->ai_family, hostList->ai_socktype, hostList->ai_protocol);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
}

void SocketConnector::makeBind() {
    bind(sockfd, hostList->ai_addr, hostList->ai_addrlen);
}

void SocketConnector::freeHostsList() {
    freeaddrinfo(hostList);
}

void SocketConnector::makeListen() {
    listen(sockfd, 4);
}

ssize_t SocketConnector::getResponse(void *buff, size_t size) {
    ssize_t received = recv(clientfd, buff, size, 0);
    if (received == -1) {
        std::cerr << "Receiving error" << std::endl;
    } else if (received == 0) {
        std::cerr << "Client closed connection" << std::endl;
    }

    return received;
}

ssize_t SocketConnector::getResponse(Packet *&packet) {
    uint8_t tmp[256];
    ssize_t receivedH = recv(clientfd, tmp, 11, 0);
    if (receivedH == -1) {
        std::cerr << "Receiving header error" << std::endl;
        return receivedH;
    } else if (receivedH == 0) {
        std::cerr << "Client closed connection" << std::endl;
        return receivedH;
    }

    packet = new Packet(tmp);
    ssize_t receivedD = recv(clientfd, tmp, packet->getDataSize(), 0);
    packet->putData(tmp, packet->getDataSize());
    if (receivedD == -1) {
        std::cerr << "Receiving data error" << std::endl;
    } else if (receivedD == 0) {
        std::cerr << "Client closed connection" << std::endl;
    }

    return receivedH + receivedD;
}

void SocketConnector::makeAccept() {
    sckSize = sizeof(clientAddr);
    clientfd = accept(sockfd, (sockaddr*)&clientAddr, &sckSize);
    if (clientfd == -1){
        exit(0);
    }
}

ssize_t SocketConnector::sendMessage(std::string msg) {
    ssize_t bytesSent =
            send(clientfd, msg.c_str(), msg.length(), 0);

    return bytesSent;
}

ssize_t SocketConnector::sendMessage(Packet *packet) {
    ssize_t bytesSent
            = send(clientfd, packet, (size_t)packet->getSize(), 0);

    return bytesSent;
}

void SocketConnector::closeClientSocket() {
    close(clientfd);
}

std::string SocketConnector::getReadableClientAddr() {
    char s[INET_ADDRSTRLEN];
    inet_ntop(clientAddr.ss_family,
              &((sockaddr_in*)&clientAddr)->sin_addr, s, sizeof(s));
    std::string addr(s);
    return addr;
}