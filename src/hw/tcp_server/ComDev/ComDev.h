#ifndef COMDEV_H
#define COMDEV_H

#pragma once
#include <tcp_server.h>
#include <thread>

class ComDev
{
public:
    ComDev(tcp_server *server);
    ~ComDev();
    void sendData(int *cardNum);

private:
    tcp_server *cardTcpServer;
};

#endif