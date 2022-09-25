#include "ComDev.h"

ComDev::ComDev(tcp_server *server)
{
    // cardTcpServer = new tcp_server(5100);
    cardTcpServer = server;
    // std::thread threadFunc(serverThread, cardTcpServer);
}

ComDev::~ComDev()
{
    delete cardTcpServer;
}

void ComDev::sendData(int *cardNum)
{
    char sendBuff[1000];
    sprintf(sendBuff, "%02x-%02x-%02x-%02x-%02x", cardNum[0], cardNum[1], cardNum[2], cardNum[3], cardNum[4]);
    cardTcpServer->sendData(sendBuff, strlen(sendBuff));
}