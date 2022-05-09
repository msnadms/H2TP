//
// Created by mason on 5/7/2022.
//
#include <winsock.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "HTTPFactory.h"

constexpr int port = 25469;
constexpr int qlen = 5;

void runServer();
void thread_helper(SOCKET sock);
void dispatchHTTP(SOCKET sock);

int main() {
    runServer();
}

void runServer() {

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return;
    }

    struct sockaddr_in serverIP{};
    memset(&serverIP, 0, sizeof(serverIP));
    serverIP.sin_family = AF_INET;
    serverIP.sin_addr.s_addr = INADDR_ANY;
    serverIP.sin_port = htons((u_short) port);

    int master = (int) socket(PF_INET, SOCK_STREAM, 0);
    if (master < 0) {
        perror("socket");
        exit(-1);
    }

    int ov = 1;
    setsockopt(master, SOL_SOCKET, SO_REUSEADDR, (char *) &ov, sizeof(int));

    int error = bind(master, (struct sockaddr *) &serverIP, sizeof(serverIP));
    if (error) {
        perror("bind");
        exit(-1);
    }

    error = listen(master, qlen);
    if (error) {
        perror("listen");
        exit(-1);
    }

    while (true) {

        struct sockaddr_in clientIP{};
        int len = sizeof(clientIP);
        SOCKET slave = (int) accept(master, (struct sockaddr *) &clientIP, &len);
        if (slave < 0) {
            perror("accept");
            break;
        }

        pthread_t thread;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(&thread, &attr, (void * (*)(void *)) thread_helper, (void *) slave);

    }

}

void dispatchHTTP(SOCKET sock) {

    const int MaxRequest = 4096;
    char request[MaxRequest + 1];
    int requestLength = 0;
    unsigned char rc;

    //Read the request from the client
    while (requestLength < MaxRequest &&
           recv(sock, (char *) &rc, sizeof(rc), 0) > 0) {

        request[requestLength] = (char) rc;
        requestLength++;

        if (requestLength > 4 &&
                request[requestLength - 4] == '\r' &&
                request[requestLength - 3] == '\n' &&
                request[requestLength - 2] == '\r' &&
                request[requestLength - 1] == '\n') {
            break;
        }
    }
    request[requestLength] = '\0';
    std::string ask = std::string(request);

    auto basic = HTTPFactory::makeHTTP(BASIC);
    basic->generateResponse("text/html");
    basic->writeResponse(sock);
}

void thread_helper(SOCKET sock) {
    dispatchHTTP(sock);
    closesocket(sock);
}