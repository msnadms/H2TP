//
// Created by mason on 5/9/2022.
//

#include <cstdint>
#include <winsock.h>
#include <iostream>
#include "Websocket.h"

const char * op_map[] = {"continue", "text", "binary",
                         "", "", "", "", "",
                         "close", "ping", "pong"};

constexpr int bufferSize = 16 * 1024;

constexpr int lsb_mask = 0xFF;

Websocket::Websocket() = default;

int Websocket::sendData(const uint8_t * data, Opcode op) {


    return 0;
}

Opcode Websocket::receiveData(SOCKET fd, std::vector<uint8_t> & message) {

    size_t currentSize = payload.size();
    payload.resize(currentSize + bufferSize);
    recv(fd, reinterpret_cast<char *>(&payload[currentSize]), bufferSize, 0);
    obits beginning_bits = *(obits *) &payload[currentSize];
    if (!!(beginning_bits.rsv)) {
        //Deal with potential negotiation later. All rsv should be zero.
        WS_LOG.LOG(WARNING, "Reserve bits not all zero.");
        return NO_OPCODE;
    }
    auto msgType = static_cast<Opcode>(beginning_bits.opcode);
    size_t cont = currentSize + 1; //keeps track of "cursor" position
    hbits mask_and_length = *(hbits *) &payload[cont];
    if ((int) mask_and_length.mask == 0) {
        //Close connection, client sent unmasked frame
        WS_LOG.LOG(ERR, "Client sent unmasked frame.");
        closesocket(fd);
        return NO_OPCODE;
    }
    if (msgType == NO_OPCODE) {
        //unknown opcode given to server
        WS_LOG.LOG(WARNING, "Message has unknown opcode");
        return msgType;
    }
    cont++;
    //Figure out payload length
    uint64_t payloadLength = mask_and_length.length;
    if (payloadLength == 126) { //use 16 bits
        payloadLength = concatBits<uint64_t>(2, cont);
        cont += 2;
    } else if (payloadLength == 127) { //use 64 bits
        payloadLength = concatBits<uint64_t>(8, cont);
        cont += 8;
    }
    auto maskingKey = concatBits<uint32_t>(4, cont);
    cont += 4; //go to payload data

    message.clear();
    message.reserve(payloadLength);
    unsigned int key_idx;
    for (unsigned int i = 0; i < payloadLength; i++) {
        key_idx = maskingKey >> (8 * (i % 4)); //unmask data
        message.push_back((payload[cont + i] ^ key_idx) & lsb_mask);
    }
    std::string log_str = std::string("Received frame with opcode: ") + op_map[msgType];
    WS_LOG.LOG(INFO, log_str.c_str());
    return msgType;
}

template <typename T>
T Websocket::concatBits(int groups, size_t idx) {
    T f_int = payload[idx];
    f_int <<= (8 * (groups - 1));
    for (int i = 1; i <= groups; i++) {
        f_int |= ((T) payload[idx + i] << (8 * (groups - i - 1)));
    }
    if (!f_int) {
        WS_LOG.LOG(WARNING, "Length bits are zero");
    }
    return f_int;
}