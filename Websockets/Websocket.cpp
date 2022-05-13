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

    size_t cont = currentSize + 1;
    hbits mask_and_length = *(hbits *) &payload[cont];
    if (mask_and_length.mask == 0) {
        //Close connection, client sent unmasked frame
        WS_LOG.LOG(ERR, "Client sent unmasked frame.");
        closesocket(fd);
    }

    //Figure out payload length
    uint64_t pplen = mask_and_length.length;
    if (pplen == 126) { //use 16 bits
        pplen = (pplen << 15) |
                (payload[currentSize + 2] << 8) |
                payload[currentSize + 3];
    } else if (pplen == 127) { //use 48 bits

    }





    return nullptr;
}