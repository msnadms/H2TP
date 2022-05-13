//
// Created by mason on 5/9/2022.
//

#include <cstdint>
#include <winsock.h>
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
    hbits mask_and_length = *(hbits *) &payload[currentSize + 1];
    if (mask_and_length.mask == 0) {
        //Close connection, client sent unmasked frame

    }
    int pplen = mask_and_length.length;
    if (pplen == 126) {

    }





    return nullptr;
}