//
// Created by mason on 5/9/2022.
//

#ifndef H2TP_WEBSOCKET_H
#define H2TP_WEBSOCKET_H

#include <cstdint>
#include <vector>

namespace Websockets {

struct hbits {
    uint8_t mask : 1;
    uint8_t length : 7;
};

class Websocket {

private:
    std::vector<uint8_t> payload;

public:
    Websocket();
    inline std::vector<uint8_t> getData() {
        return payload;
    }

    int sendData(const uint8_t * data);
    const uint8_t * receiveData(SOCKET fd);










};

}
#endif //H2TP_WEBSOCKET_H