//
// Created by mason on 5/7/2022.
//

#include <unistd.h>
#include <winsock.h>
#include <iostream>
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
    if (response) {
        send(fd, response, (int) strlen(response), 0);
    }
}

const char * BasicHTTP::generateResponse(const char * mime){
    const char * header = "HTTP/1.1 200 Document follows\r\n"
                          "Server: H2TP\r\n"
                          "Content-type: %s\r\n"
                          "\r\n";
    int size = (int) (strlen(header) + strlen(mime)) + 1;
    char * buff = (char *) malloc(size);
    sprintf(buff, header, mime);
    response = buff;
    return buff;
}

int BasicHTTP::transmitFile(int fd, int file) {
    char buffer[1024];
    int n;
    while ((n = recv(file, buffer, 1024, 0)) > 0) {
        int s = send(fd, buffer, n, 0);
        if (s < 1) {
            return -1;
        }
    }
    return n;
}

const char * AuthHTTP::generateResponse(const char * mime) {
    const char * header = "HTTP/1.1 401 Unauthorized\r\n"
                          "WWW-Authenticate: Basic realm=\"h2tp-verify\""
                          "\r\n\r\n";
    response = header;
    return header;
}

const char * ErrHTTP::generateResponse(const char * mime) {
    const char * header = "HTTP/1.1 404 File Not Found\r\n"
                          "Server: H2TP\r\n"
                          "Content-type: text/plain\r\n\r\n"
                          "Could not find file: %s. The server returned an error.";
    int size = (int) (strlen(header) + strlen(mime)) + 1;
    char * buff = (char *) malloc(size);
    sprintf(buff, header, mime);
    response = buff;
    return (const char *) buff;
}

const char * CgiHTTP::generateResponse(const char * mime) {
    const char * header = "HTTP/1.1 Document follows\r\n";
    response = header;
    return header;
}