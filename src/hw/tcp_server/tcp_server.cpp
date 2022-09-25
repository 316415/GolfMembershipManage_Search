#include "tcp_server.h"

tcp_server::tcp_server(int port)
{
    portNum = port;
    clientState = false;
    createSocket();
}

tcp_server::~tcp_server()
{
    close(serverSock_fd);
}

int tcp_server::getServerSocket()
{
    return serverSock_fd;
}

int tcp_server::getClientSocket()
{
    return clientSock_fd;
}

void tcp_server::createSocket()
{
    if ((serverSock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //AF_INET 인터넷임. 이거만 씀, TCP 옵션이라 SOCK_STREAM임. UDP는 다름
    {
        perror("socket()");
        return ; //0은 정상 0이 아니면 에러.
    }
    //?? 무슨 기능이라고?? 서버 바로 다시 켜는 기능?
    int option = 1;
    setsockopt(serverSock_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)); //SO_REUSEADDR 해당 주소를 재사용할 거다

    /* 주소 구조체에 주소 저장*/
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //일단 p.346부터 읽어보자. 구조체 값은 바뀌는 경우가 많지 않다고 함.
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY : server가 가진 all local ip를 할당하겠다.
    serv_addr.sin_port = htons(portNum); /* 사용할 포트 지정 */

    /* bind 함수를 사용하여 서버 소켓의 주소 설정 */
    if(bind(serverSock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // OS에게 서버 할당하겠다고(메모리 점유하겠다고) bind 함수로 선언
    {
        perror("bind()");
        return ;
    }

    /*동시에 접속하는 클라이언트의 처리를 위한 대기 큐를 설정*/
    if(listen(serverSock_fd, 8) < 0) //8개까지 처리하기 위해서 buffer를 만들 거야.라는 의미.
    {
        perror("listen()");
        return ;
    }
}

int tcp_server::waitAccept()
{
    char mesg[BUFSIZ];
    clnt_addr_size = sizeof(client_addr);
    clientSock_fd = accept(serverSock_fd, (struct sockaddr *)&client_addr, &clnt_addr_size); //client가 connect할 때까지 대기함) accept하면서 client의 주소 포트가 같이 들어감
    //accept 실패로 에러 나면 -1을 줌
    if(clientSock_fd < 0)
    {
        perror("accept() error\n");
        return -1;
    }
    
    /*네트워크 주소를 문자열로 변경 */
    inet_ntop(AF_INET, &client_addr.sin_addr, mesg, BUFSIZ);
    printf("Client is connected : %s\n", mesg);

    return clientSock_fd;
}


int tcp_server::recvData(char *recvBuff, size_t size)
{
    int len = read(clientSock_fd, recvBuff, size);
    return len;
}


int tcp_server::sendData(char *sendBuff, size_t size)
{
    if(getClientState())
    {
        int len = write(clientSock_fd, sendBuff, size);
        return len;
    }
    else return -1;
}

void tcp_server::closeSocket(int socket)
{
    close(socket);
}

bool tcp_server::getClientState()
{
    return clientState;
}
void tcp_server::setClientState(bool state)
{
    clientState = state;
}