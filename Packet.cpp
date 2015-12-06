//
// Created by pawkrol on 12/6/15.
//

#include <time.h>

#include "Packet.h"

uint16_t Packet::id = 0;

Packet::Packet() {
    id++;
    packetId = id;
    *(uint16_t*)buff = packetId;
    *(uint64_t*)(buff + 2) = (uint64_t)time(NULL);
}

Packet::Packet(uint8_t *recv) {
    packetId = *(uint16_t*)buff = *(uint16_t*)recv;
    *(uint64_t*)(buff + 2) = *(uint64_t*)(recv + 2);
    *(buff + 10) = *(recv + 10);
}

void Packet::putData(std::string data) {
    if (data.length() > 255){
        std::cerr << "too much data" << std::endl;
        *(buff + 10) = '\0';
    } else {
        this->data = data;
        size += data.length() + 1;
        *(buff + 10) = (uint8_t) (data.length() + 1);

        for (unsigned int i = 0; i < data.length(); i++){
            *(buff + 11 + i) = (uint8_t) data.at(i);
        }
        *(buff + 10 + data.length() + 1) = '\0';
    }
}

void Packet::putData(uint8_t *data, size_t length) {
    if (length > 255){
        std::cerr << "too much data" << std::endl;
    } else {
        std::string _data((char*)data, length);
        this->data = _data;
    }
}

uint16_t Packet::getId() {
    return packetId;
}

uint64_t Packet::getTime() {
    return *(uint64_t*)(buff + 2);
}

std::string Packet::getData() {
    return data;
}

uint8_t Packet::getDataSize() {
    return *(buff + 10);
}

int Packet::getSize() {
    return size;
}