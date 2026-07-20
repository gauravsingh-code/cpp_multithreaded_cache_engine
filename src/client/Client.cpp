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
    
     //conver ipv4 string "[IP_ADDRESS]" to binary format 
     // this function convertsPresentation format (what we write) to Network format(what the computer understand)
     if(inet_pton(AF_INET,"127.0.0.1", &serverAddress.sin_addr) <= 0){
        std::cout<<"Invalid server address/IP format"<<std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
     }


    //4.connect to server
    //reinterpret_cast is used to convert the pointer type of serverAddress to sockaddr_in* to match the connect() function's expected argument type.
    if(connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR){
        std::cout<<"Connection to server failed with error: "<<WSAGetLastError()<<std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    std::cout<<"Connected to server successfully! "<<std::endl;
    
    // const char *message = "Hello server\n ";
    // int bytesSent = send(clientSocket, message, static_cast<int>(strlen(message)), 0);

    //sending data to server
    while(true){
        
        std::string message;
        std::cout<<"Enter the message to send to server: ";
        std::getline(std::cin, message);

        int bytesSent = send(clientSocket, message.c_str(), static_cast<int>(message.length()), 0);
        if(bytesSent == SOCKET_ERROR){
            std::cout<<"Failed to send data\n";
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
        std::cout<<"Data sent successfully.\n"<<bytesSent<<" bytes"<<std::endl; 

        //lets's receive the data from server
        char buffer[1024] = {0};
        int bytesReceived = recv(clientSocket , buffer, sizeof(buffer) -1, 0);
    
        if(bytesReceived == SOCKET_ERROR){
            std::cout<<"Failed to receive response\n";
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    
        //setting the last of string to null string like a c string
        buffer[bytesReceived] = '\0';
        
        std::cout<<"Server says: "<<buffer <<'\n';
    }
    
    //cleanup when done
    closesocket(clientSocket);
    WSACleanup();

    return 0;
    
}