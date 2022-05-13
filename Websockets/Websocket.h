//
// Created by mason on 5/9/2022.
//

#ifndef H2TP_WEBSOCKET_H
#define H2TP_WEBSOCKET_H

#include <cstdint>
#include <vector>
#include "../Log.h"

namespace Websockets {

struct hbits {
    uint8_t mask : 1;
    uint8_t length : 7;
};

constexpr uint8_t r_mask = 0x70;
struct obits {
    uint8_t fin : 1;
    uint8_t rsv : 3;
    uint8_t opcode : 4;
};

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

    int sendData(const uint8_t * data);
    const uint8_t * receiveData(SOCKET fd);

};

}
#endif //H2TP_WEBSOCKET_H