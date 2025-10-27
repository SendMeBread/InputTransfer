#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>

#include <windows.h>

#include "../sendMouseDataWin.hpp"

WSADATA wsaData;

std::string port;
std::string ip;

int main(int argc, char* argv[]) {
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    std::string port = argv[2];
    std::string ip = argv[1];

    struct addrinfo *result = NULL, *ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(ip.c_str(), port.c_str(), &hints, &result) < 0) {
        std::cerr << "Connection failed..." << std::endl;
        WSACleanup();
        return 1;
    }

    SOCKET host_sock = INVALID_SOCKET;

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        host_sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (host_sock == INVALID_SOCKET) {
            std::cerr << "Invalid socket..." << std::endl;
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }

        if (connect(host_sock, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
            std::cerr << "Socket error..." << std::endl;
            closesocket(host_sock);
            host_sock = INVALID_SOCKET;
        }
        
        break;
    }

    freeaddrinfo(result);

    if (host_sock == INVALID_SOCKET) {
        std::cerr << "Unable to connect to server!" << std::endl;
        WSACleanup();
        return 1;
    }

    sendMouseData cursor(host_sock);
    
    while (true) {
        if (!cursor.sendCursor()) {
            WSACleanup();
            closesocket(host_sock);
            return 1;
        }
        usleep(500);
    }
    
    if (shutdown(host_sock, SD_SEND) == SOCKET_ERROR) {
        std::cerr << "Failed to close properly..." << std::endl;
        closesocket(host_sock);
        host_sock = INVALID_SOCKET;
        return 1;
    }

    closesocket(host_sock);
    WSACleanup();

    return 0;
}