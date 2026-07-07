#include<iostream>

#include "TcpServer.h"

TcpServer::TcpServer(int port) : port_(port) , listenSocket_(INVALID_SOCKET){};

TcpServer::~TcpServer(){
    if(listenSocket_ != INVALID_SOCKET){
        closesocket(listenSocket_);
    }
    WSACleanup();
}

bool TcpServer::start(){
    if(WSAStartup(MAKEWORD(2,2), &wsaData_) != 0){
        std::cout<<"Failed to initialize Winsock."<<std::endl;
        return false;
    }
    std::cout<<"Winsock initialized successfully."<<std::endl;
    return true;
}



