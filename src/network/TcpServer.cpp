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
    //Address family, type of socket, protocol
    listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  

    //Socket is just like the phone without the sim 
    if(listenSocket_ == INVALID_SOCKET){
        std::cout<<"Failed to create the socket\n";
        WSACleanup();   //free the socket and unloads the networking librry   
        return false;
    }

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET; //IPv4
    serverAddress.sin_addr.s_addr = INADDR_ANY; //bind to all available interfaces
    serverAddress.sin_port = htons(port_); //convert port to network byte order

    if(bind(listenSocket_, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR){
        std::cout<<"Bind Failed with error: "<<WSAGetLastError()<<std::endl;
        closesocket(listenSocket_);
        WSACleanup();
        return false;
    }

    if(listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR){
        std::cout<<"Listen Failed with error: "<<WSAGetLastError()<<std::endl;
        closesocket(listenSocket_);
        WSACleanup();
        return false;
    }

    std::cout<<"Waiting for a client to connect on port "<<port_<<"...\n";

    clientSocket_ = accept(listenSocket_, nullptr, nullptr);  //accept the connection from the client

    if(clientSocket_ == INVALID_SOCKET){
        std::cout<<"Failed to accept the connection from the client\n";
        // closesocket(listenSocket_);
        // WSACleanup();
        return false;
    }


    std::cout<<"Client Connected successfully .\n";    
    return true;
}



