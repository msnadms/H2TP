//
// Created by mason on 5/13/2022.
//

#include "Log.h"
#include <stdexcept>
#include <sstream>
#include <ctime>
#include <cstring>

Log::Log() {
    file_path = "/logs/log.txt";
    file = fopen(file_path, "w");
    if (!file) {
        throw std::invalid_argument("File for logging not found");
    }
}

Log::Log(const char * path) {
    file_path = path;
    file = fopen(file_path, "w");
    if (!file) {
        throw std::invalid_argument("File for logging not found");
    }
}

void Log::changeFile(const char * path) {
    file_path = path;
    fclose(file);
    file = fopen(file_path, "w");
    if (!file) {
        throw std::invalid_argument("File for logging not found");
    }
}

void Log::LOG(Defcon d, const char * message) {
    std::ostringstream oss;
    oss << logs << ". [ " << std::time(nullptr) << " ]"
        << "[ " << getpid() << " ]"
        << "[ " << str_map[d] << " ] "
        << message << "\n";
    std::string str_w = oss.str();
    size_t bw = fwrite(str_w.c_str(), 1, strlen(str_w.c_str()), file);
    if (bw != strlen(str_w.c_str())) {
        throw std::invalid_argument("Could not write to logging file");
    } else {
        logs++;
    }
}

Log::~Log() {
    fclose(file);
}