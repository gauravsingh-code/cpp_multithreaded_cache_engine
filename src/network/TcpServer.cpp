#include<iostream>

#include "TcpServer.h"

TcpServer::TcpServer(int port, commandProcessor &processor) : port_(port) , listenSocket_(INVALID_SOCKET), clientSocket_(INVALID_SOCKET), processor_(processor) {};

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
   
    std::cout<<"Waiting for a client to connect on port "<<port_<<"...\n";

    //we want that the server should keep running and accepting the clients until we close it
    while(true){
        clientSocket_ = accept(listenSocket_, nullptr, nullptr);  //accept the connection from the client

        if(clientSocket_ == INVALID_SOCKET){
            std::cout<<"Failed to accept the connection from the client\n";
            closesocket(listenSocket_);
            WSACleanup();
            continue; //continue to accept the next client
        }
        std::cout<<"Client Connected successfully .\n";

        // while(true){
        //     char buffer[1024] = {0};
            
        //     //receiving the data sent by the client
        //     int byteReceived = recv(clientSocket_, buffer, sizeof(buffer) -1, 0);
            
        //     if(byteReceived == SOCKET_ERROR){
        //         std::cout<<"Failed to receive data: " << WSAGetLastError()<<std::endl;
        //         break; //break the inner loop to accept the next client
        //     }else if(byteReceived == 0){
        //         std::cout<<"Client disconnected gracefully\n";
        //         break; //break the inner loop to accept the next client
        //     }else{
        //         buffer[byteReceived] = '\0';
        //         std::cout<<"Received: "<<buffer<<std::endl;
        //     }

        //     //sendigng response to the client
        //     std::string response = processor_.execute(buffer); //process the command and get the response
        //     std::cout<<"Response from commandProcessor: "<<response<<std::endl;
        //     const char *message = response.c_str();  //c style response string with a null pointer to send to the client 

        //     int bytesSent = send(clientSocket_ , message , strlen(message) , 0);
        //     if(bytesSent == SOCKET_ERROR){
        //         std::cout<<"Failed to send data: " << WSAGetLastError()<<std::endl;
        //         break; //break the inner loop to accept the next client
        //     }

        //     std::cout<<"Data sent successfully FROM SERVER: \n"<<message<<std::endl;  
        // }

    }

    // clientSocket_ = accept(listenSocket_, nullptr, nullptr);  //accept the connection from the client

    // if(clientSocket_ == INVALID_SOCKET){
    //     std::cout<<"Failed to accept the connection from the client\n";
    //     // closesocket(listenSocket_);
    //     // WSACleanup();
    //     return false;
    // }
    // std::cout<<"Client Connected successfully .\n";    

    // char buffer[1024] = {0};
    
    // //receiving the data sent by the client
    // int byteReceived = recv(clientSocket_, buffer, sizeof(buffer) -1, 0);
    
    // if(byteReceived == SOCKET_ERROR){
    //     std::cout<<"Failed to receive data: " << WSAGetLastError()<<std::endl;
    //     return false;
    // }else if(byteReceived == 0){
    //     std::cout<<"Client disconnected gracefully\n";
    //     return false;   
    // }else{
    //     buffer[byteReceived] = '\0';
    //     std::cout<<"Received: "<<buffer<<std::endl;
    // }

    // //sendigng response to the client
    // // const char *message = "Hey, hw are yu clinet ?";
    // std::string response = processor_.execute(buffer); //process the command and get the response
    // std::cout<<"Response from commandProcessor: "<<response<<std::endl;
    // const char *message = response.c_str();  //c style response string with a null pointer to send to the client 

    // int bytesSent = send(clientSocket_ , message , strlen(message) , 0);
    // if(bytesSent == SOCKET_ERROR){
    //     std::cout<<"Failed to send data: " << WSAGetLastError()<<std::endl;
    //     return false;
    // }

    // std::cout<<"Data sent successfully FROM SERVER: \n"<<message<<std::endl;  
    return true;
}


bool TcpServer::initialize(){
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
        return true;
}

void TcpServer::handleClient(SOCKET clientSocket){
    while(true){
        char buffer[1024] = {0};    
        //receiving the data sent by the client
        int byteReceived = recv(clientSocket, buffer, sizeof(buffer) -1, 0);
        
        if(byteReceived == SOCKET_ERROR){
            std::cout<<"Failed to receive data: " << WSAGetLastError()<<std::endl;
            break;
        }else if(byteReceived == 0){
            std::cout<<"Client disconnected gracefully\n";
            break;   
        }else{
            buffer[byteReceived] = '\0';
            std::cout<<"Received: "<<buffer<<std::endl;
        }

        //sendigng response to the client
        // const char *message = "Hey, hw are yu clinet ?";
        std::string response = processor_.execute(buffer); //process the command and get the response
        std::cout<<"Response from commandProcessor: "<<response<<std::endl;
        const char *message = response.c_str();  //c style response string with a null pointer to send to the client 

        int bytesSent = send(clientSocket , message , strlen(message) , 0);
        if(bytesSent == SOCKET_ERROR){
            std::cout<<"Failed to send data: " << WSAGetLastError()<<std::endl;
            break;
        }

        std::cout<<"Data sent successfully FROM SERVER: \n"<<message<<std::endl;  
    }

    //sendigng response to the client
    // const char *message = "Hey, hw are yu clinet ?";
    std::string response = processor_.execute(buffer); //process the command and get the response
    std::cout<<"Response from commandProcessor: "<<response<<std::endl;
    const char *message = response.c_str();  //c style response string with a null pointer to send to the client 

    int bytesSent = send(clientSocket , message , strlen(message) , 0);
    if(bytesSent == SOCKET_ERROR){
        std::cout<<"Failed to send data: " << WSAGetLastError()<<std::endl;
        return;
    }

    std::cout<<"Data sent successfully FROM SERVER: \n"<<message<<std::endl;  
    closesocket(clientSocket);

}

