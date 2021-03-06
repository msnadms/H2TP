/*
      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     +-+-+-+-+-------+-+-------------+-------------------------------+
     |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
     |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
     |N|V|V|V|       |S|             |   (if payload len==126/127)   |
     | |1|2|3|       |K|             |                               |
     +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
     |     Extended payload length continued, if payload len == 127  |
     + - - - - - - - - - - - - - - - +-------------------------------+
     |                               |Masking-key, if MASK set to 1  |
     +-------------------------------+-------------------------------+
     | Masking-key (continued)       |          Payload Data         |
     +-------------------------------- - - - - - - - - - - - - - - - +
     :                     Payload Data continued ...                :
     + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
     |                     Payload Data continued ...                |
     +---------------------------------------------------------------+
     Websocket frame reference.
     Source: https://datatracker.ietf.org/doc/html/rfc6455/
*/

#ifndef H2TP_WEBSOCKET_H
#define H2TP_WEBSOCKET_H

#include <cstdint>
#include <vector>
#include "../Log.h"
#include "Parsing.h"

class Websocket {

private:
    std::vector<uint8_t> payload;
    Log WS_LOG;
    template <typename T>
    T concatBits(int groups, size_t idx);
public:
    Websocket();
    inline std::vector<uint8_t> getData() {
        return payload;
    }

    int sendData(const uint8_t * data, Opcode op);
    Opcode receiveData(SOCKET fd, std::vector<uint8_t> & message);
};

#endif //H2TP_WEBSOCKET_H