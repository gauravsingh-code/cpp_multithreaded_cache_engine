#include<iostream>

#include<winsock2.h>
#include<ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main(){
    WSADATA wsaData;

    //this checks the specific version of winsock library here 2.2 and success returns 0
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        std::cout << "Failed to initialize Winsock." << std::endl;
        return 1;  
    }

    std::cout << "Winsock initialized successfully." << std::endl;

    //creating socker for the client
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocket == INVALID_SOCKET){
        std::cout << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;  
    }

    std::cout<< "Socket created successfully." << std::endl;

     // 3. Configure Server Address Structure (Targeting 127.0.0.1:8080)
     sockaddr_in serverAddress{};
     serverAddress.sin_family = AF_INET;
     serverAddress.sin_port = htons(8080);  //port as per server's
    
     //conver ipv4 string "127.0.0.1" to binary format
     if(inet_pton(AF_INET,"127.0.0.1", &serverAddress.sin_addr) <= 0){
        std::cout<<"Invalid server address/IP format"<<std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
     }


    //4.connect to server
    if(connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR){
        std::cout<<"Connection to server failed with error: "<<WSAGetLastError()<<std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    std::cout<<"Connected to server successfully! "<<std::endl;
    
    //cleanup when done
    closesocket(clientSocket);
    WSACleanup();

    return 0;
    
}