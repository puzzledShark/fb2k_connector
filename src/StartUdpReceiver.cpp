/*
* Basically sets up a UDP Reciever socket to take in messages from the middle man
* UDP because neither the middle man, nor this component care about data integrity
* all these messages are basically just commands directly run on foobar, data loss is acceptable
*/
#include <iostream>
#include <winsock2.h>
#include "stdafx.h"
#include <foobar2000/SDK/foobar2000.h>
#include "StartUdpReceiver.h"
#include "player_functions.h"


#pragma comment(lib, "Ws2_32.lib")

const int BUFFER_SIZE = 1024;


// Function to start the UDP receiver
int StartUdpReceiver(int port) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        console::print("Error initializing Winsock");
        return 1;
    }

    // Create a UDP socket
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        console::print("Error creating socket");
        WSACleanup();
        return 1;
    }

    // Bind the socket to the port
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        console::print("Error binding socket");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    console::print("Receiver listening on port %d", port);

    char buffer[BUFFER_SIZE];
    struct sockaddr_in clientAddr;
    int clientLen;
    int recvLen;

    // Receive and print messages indefinitely
    while (true) {
        clientLen = sizeof(clientAddr);
        recvLen = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
            (struct sockaddr*)&clientAddr, &clientLen);
        if (recvLen == SOCKET_ERROR) {
            console::print("Error receiving message");
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
        buffer[recvLen] = '\0';
        
        char temp[BUFFER_SIZE];
        sprintf(temp, "Received message: %s from %s", buffer, inet_ntoa(clientAddr.sin_addr));
        console::print(temp);

        std::string command(buffer, recvLen);
        command_queue.push(command);
        
        cv.notify_one();

        // This is commented out, because it was when i learned that foobar really hates it when you run this command on a separate thread and it nukes itself lol
        //pbc->play_or_pause();
        
    }

    // Close the socket (unreachable in this example)
    closesocket(sockfd);
    WSACleanup();

    return 0;
}


void MyMainThreadFunction() {
    // Perform actions on the main thread
    static_api_ptr_t<playback_control> pbc;
    pbc->play_or_pause();
}