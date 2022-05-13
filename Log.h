//
// Created by mason on 5/13/2022.
//

#ifndef H2TP_LOG_H
#define H2TP_LOG_H

#include <cstdio>

enum Defcon {
    DEBUG,
    INFO,
    WARNING,
    ERR
};
static const char * str_map[] = {"Debug", "Info", "Warning", "Error"};

class Log {

private:
    const char * file_path;
    static int logs;
    FILE * file;
public:
    Log();
    explicit Log(const char * path);
    void changeFile(const char * path);
    void LOG(Defcon d, const char * message);
    ~Log();
};


#endif //H2TP_LOG_H