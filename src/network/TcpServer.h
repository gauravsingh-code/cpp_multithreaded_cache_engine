#pragma once

#include<winsock2.h>
#include<ws2tcpip.h>


class TcpServer{
    public:
        TcpServer(int port);

        ~TcpServer();

        bool start();

    private:
        int port_;
        SOCKET listenSocket_;
        WSADATA wsaData_    ;
};

    //winsock start
        // int start

    //socket