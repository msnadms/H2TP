//
// Created by mason on 5/13/2022.
//

#ifndef H2TP_LOG_H
#define H2TP_LOG_H

#include <cstdio>

enum Defcon {
    INFO,
    WARNING,
    ERR
};
static const char * str_map[] = {"Info", "Warning", "Error"};

class Log {

private:
    const char * file_path;
    FILE * file;
public:
    Log();
    [[maybe_unused]] explicit Log(const char * path);
    [[maybe_unused]] void changeFile(const char * path);
    void LOG(Defcon d, const char * message, bool debug = false);
    ~Log();
};


#endif //H2TP_LOG_H