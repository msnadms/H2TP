//
// Created by mason on 5/7/2022.
//

#ifndef H2TP_HTTPFACTORY_H
#define H2TP_HTTPFACTORY_H

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <winsock.h>

enum ResponseType {
        BASIC,
        AUTH,
        ERR,
        CGI
};

class HTTPFactory {
public:
    std::string response;
    static std::unique_ptr<HTTPFactory> makeHTTP(ResponseType);

    virtual FILE * prepareFile(const char * path) {};
    virtual std::string generateResponse(const char * mime) = 0;

    void writeResponse(SOCKET fd) const;
};

class BasicHTTP : public HTTPFactory {
    FILE * file{};
public:
    std::string generateResponse(const char * mime) override;
    FILE * prepareFile(const char * path) override;
    int transmitFile(SOCKET fd);
};

class AuthHTTP : public HTTPFactory {
public:
    std::string generateResponse(const char * mime) override;
};

class ErrHTTP : public HTTPFactory {
public:
    std::string generateResponse(const char * mime) override;
};

class CgiHTTP : public HTTPFactory {
public:
    std::string generateResponse(const char * mime) override;
};

#endif //H2TP_HTTPFACTORY_H