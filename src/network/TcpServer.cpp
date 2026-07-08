#include<iostream>

#include "TcpServer.h"

TcpServer::TcpServer(int port) : port_(port) , listenSocket_(INVALID_SOCKET), clientSocket_(INVALID_SOCKET) {};

TcpServer::~TcpServer(){
    if(listenSocket_ != INVALID_SOCKET){
        closesocket(listenSocket_);
    }
    if(clientSocket_ != INVALID_SOCKET){
        closesocket(clientSocket_);
    }
    WSACleanup();
}

bool TcpServer::start(){
    if(WSAStartup(MAKEWORD(2,2), &wsaData_) != 0){
        std::cout<<"Failed to initialize Winsock."<<std::endl;
        return false;
    }
    std::cout<<"Winsock initialized successfully."<<std::endl;

    //this is gonna actually returna a socket 
    // A gate through other devices can connect to our device
    //it decided by the windows whether we get the socket or not
    listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  

    //Socket is just like the phone without the sim 
    if(listenSocket_ == INVALID_SOCKET){
        std::cout<<"Failed to create the socket\n";
        WSACleanup();   //free the socket and unloads the networking librry   
        return false;
    }

    std::cout<<"Created Socket Successfullly .\n";    
    return true;
}



