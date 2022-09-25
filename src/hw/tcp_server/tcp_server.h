#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#pragma once
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class tcp_server
{
public:
    tcp_server(int port);
    virtual ~tcp_server();
    int getServerSocket();
    int getClientSocket();
    bool getClientState();
    void setClientState(bool state);
    void createSocket();
    int waitAccept();
    int recvData(char *recvBuff, size_t size);
    int sendData(char *sendBuff, size_t size);
    void closeSocket(int socket);

private:
    int serverSock_fd;
    int clientSock_fd;
    //ip는 다 쓰니까 포트만 할당
    int portNum;
    bool clientState;
    socklen_t clnt_addr_size;

    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;
};

#endif