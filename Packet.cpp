//
// Created by pawkrol on 12/6/15.
//

#include <time.h>

#include "Packet.h"

uint16_t Packet::id = 0;

Packet::Packet() {
    id++;
    packetId = id;
    *(uint16_t*)buff = packetId; //first 16bits for packetId
    *(uint64_t*)(buff + 2) = (uint64_t)time(NULL); //next 64bits for time stamp
}

Packet::Packet(uint8_t *recv) {
    packetId = *(uint16_t*)buff = *(uint16_t*)recv;
    *(uint64_t*)(buff + 2) = *(uint64_t*)(recv + 2);
    *(buff + _dataSizeOffset) = *(recv + _dataSizeOffset); //8bits for data amount info
}

void Packet::putData(std::string data) {
    if (data.length() > _bytesLeft){
        std::cerr << "too much data" << std::endl;
        *(buff + _dataSizeOffset) = 0;
    } else {
        this->data = data;
        size += data.length() + 1;
        *(buff + _dataSizeOffset) = (uint8_t) (data.length() + 1);

        for (unsigned int i = 0; i < data.length(); i++){
            *(buff + _headerSize + i) = (uint8_t) data.at(i);
        }
        *(buff + _dataSizeOffset + data.length() + 1) = '\0';
    }
}

void Packet::putData(uint8_t *data, size_t length) {
    if (length > _bytesLeft){
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
    return *(buff + _dataSizeOffset);
}

int Packet::getSize() {
    return size;
}