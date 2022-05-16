//
// Created by mason on 5/13/2022.
//

#include "Log.h"
#include <stdexcept>
#include <sstream>
#include <ctime>
#include <cstring>
#include <iostream>

Log::Log() {
    file_path = "/logs/log.txt";
    file = fopen(file_path, "w");
    if (!file) {
        throw std::invalid_argument("File for logging not found");
    }
}

[[maybe_unused]]
Log::Log(const char * path) {
    file_path = path;
    file = fopen(file_path, "w");
    if (!file) {
        throw std::invalid_argument("File for logging not found");
    }
}

[[maybe_unused]]
void Log::changeFile(const char * path) {
    file_path = path;
    fclose(file);
    file = fopen(file_path, "w");
    if (!file) {
        throw std::invalid_argument("File for logging not found");
    }
}

void Log::LOG(Defcon d, const char * message, bool debug) {
    std::ostringstream oss;
    oss << "[ " << std::time(nullptr) << " ]"
        << "[ " << getpid() << " ]"
        << "[ " << str_map[d] << " ] "
        << message << "\n";
    std::string str_w = oss.str();
    if (debug) {
        std::cout << str_w.c_str() << std::endl;
        return;
    }
    size_t bw = fwrite(str_w.c_str(), 1, strlen(str_w.c_str()), file);
    if (bw != strlen(str_w.c_str())) {
        throw std::invalid_argument("Could not write to logging file");
    }
}

Log::~Log() {
    fclose(file);
}