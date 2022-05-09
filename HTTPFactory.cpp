//
// Created by mason on 5/7/2022.
//

#include <unistd.h>
#include <winsock.h>
#include <iostream>
#include <sstream>
#include "HTTPFactory.h"

std::unique_ptr<HTTPFactory> HTTPFactory::makeHTTP(ResponseType r) {

    if (r == BASIC) {
        return std::make_unique<BasicHTTP>();
    } else if (r == AUTH) {
        return std::make_unique<AuthHTTP>();
    } else if (r == ERR) {
        return std::make_unique<ErrHTTP>();
    } else if (r == CGI) {
        return std::make_unique<CgiHTTP>();
    }
    return nullptr;

}

void HTTPFactory::writeResponse(SOCKET fd) const {
    const char * cr = response.c_str();
    if (cr) {
        send(fd, cr, (int) strlen(cr), 0);
    }
}

std::string BasicHTTP::generateResponse(const char * mime){
    std::ostringstream oss;
    oss << "HTTP/1.1 200 Document follows\r\n"
        << "Server: H2TP\r\n"
        << "Content-type: "
        << mime
        << "\r\n\r\n";
    std::string buff = oss.str();
    response = buff;
    return buff;
}

int BasicHTTP::transmitFile(SOCKET fd) {
    char buffer[1024];
    int n;
    while ((n = (int) fread(buffer, sizeof(char), 1024, file)) > 0) {
        int s = send(fd, buffer, n, 0);
        if (s < 1) {
            return -1;
        }
    }
    fclose(file);
    return n;
}

FILE * BasicHTTP::prepareFile(const char * path) {
    FILE * f = fopen(path, "r");
    file = f;
    return f;
}

std::string AuthHTTP::generateResponse(const char * mime) {
    std::ostringstream oss;
    oss << "HTTP/1.1 401 Unauthorized\r\n"
        << "WWW-Authenticate: Basic realm=\"h2tp-verify\""
        << "\r\n\r\n";
    std::string header = oss.str();
    response = header;
    return header;
}

std::string ErrHTTP::generateResponse(const char * mime) {
    std::ostringstream oss;
    oss << "HTTP/1.1 404 File Not Found\r\n"
        << "Server: H2TP\r\n"
        << "Content-type: text/plain\r\n\r\n"
        << "Could not find file: "
        << mime
        << ". The server returned an error.";
    std::string buff = oss.str();
    response = buff;
    return buff;
}

std::string CgiHTTP::generateResponse(const char * mime) {
    std::string header = "HTTP/1.1 Document follows\r\n";
    response = header;
    return header;
}