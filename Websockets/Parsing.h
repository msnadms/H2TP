//
// Created by mason on 5/15/2022.
//

#ifndef H2TP_PARSING_H
#define H2TP_PARSING_H

enum Opcode {
    CONTINUE = 0x0,
    TEXT = 0x1,
    BINARY = 0x2,
    CLOSE = 0x8,
    PING = 0x9,
    PONG = 0xA,
    NO_OPCODE
};
const char * op_map[] = {"continue", "text", "binary", "close", "ping", "pong"};

constexpr int bufferSize = 16 * 1024;

constexpr int lsb_mask = 0xFF; //Get the 8 least significant bits of uint32_t

struct hbits {
    unsigned int length : 7;
    unsigned int mask : 1;
};

struct obits {
    unsigned int opcode : 4;
    unsigned int rsv : 3;
    unsigned int fin : 1;
};

#endif //H2TP_PARSING_H