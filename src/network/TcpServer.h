#pragma once

#include<winsock2.h>
#include<ws2tcpip.h>
#include "command/commandProcessor.h"

class TcpServer{
    public:
        TcpServer(int port, commandProcessor &processor) ;

        ~TcpServer();

        bool start();

    private:
        int port_;
        SOCKET listenSocket_;
        SOCKET clientSocket_;
        WSADATA wsaData_;

        commandProcessor &processor_;
};

    //winsock start
        // int start

    //socket