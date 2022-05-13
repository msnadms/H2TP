//
// Created by mason on 5/9/2022.
//

#include <cstdint>
#include <winsock.h>
#include <iostream>
#include "Websocket.h"

constexpr int bufferSize = 16 * 1024;

using namespace Websockets;

Websocket::Websocket() = default;

int Websocket::sendData(const uint8_t * data) {

    return 0;
}

const uint8_t * Websocket::receiveData(SOCKET fd) {

    size_t currentSize = payload.size();
    payload.resize(currentSize + bufferSize);
    recv(fd, reinterpret_cast<char *>(&payload[currentSize]), bufferSize, 0);
    obits beginning_bits = *(obits *) &payload[currentSize];
    if (beginning_bits.rsv & r_mask) {
        //Deal with potential negotiation later. All rsv should be zero.
        WS_LOG.LOG(WARNING, "Reserve bits not all zero.");
    }
    size_t cont = currentSize + 1;
    hbits mask_and_length = *(hbits *) &payload[cont];
    if (mask_and_length.mask == 0) {
        //Close connection, client sent unmasked frame
        WS_LOG.LOG(ERR, "Client sent unmasked frame.");
        closesocket(fd);
    }

    //Figure out payload length
    uint64_t payloadLength = mask_and_length.length;
    if (payloadLength == 126) { //use 16 bits
        payloadLength = ((uint64_t) payload[cont + 1] << 8) | ((uint64_t) payload[cont + 2]);
    } else if (payloadLength == 127) { //use 64 bits
        payloadLength = concatBits<uint64_t>(8, cont);
    }
    cont += 8; //go to masking key
    auto maskingKey = concatBits<uint32_t>(4, cont);



    std::cout << payloadLength << maskingKey;
    return nullptr;
}

template <typename T>
T Websocket::concatBits(int groups, size_t idx) {
    T f_int = payload[idx];
    for (int i = 1; i <= groups; i++) {
        f_int |= ((T) payload[idx + i] << (8 * (groups - i)));
    }
    if (!f_int) {
        WS_LOG.LOG(WARNING, "Length bits are zero");
    }
    return f_int;
}
