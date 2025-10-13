#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>

int main() {
    WSADATA wsaData;
    int iresult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iresult < 0) {
        std::cerr << "WSAStartup error..." << std::endl;
    }
    std::string port;
    std::string ip;
    std::string message;

    std::cout << "Please input your desired port:" << std::endl;
    std::getline(std::cin, port);

    std::cout << "Please input your desired ip:" << std::endl;
    std::getline(std::cin, ip);

    struct addrinfo *result = NULL, *ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iresult = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result);
    
    if (iresult < 0) {
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

        iresult = connect(host_sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iresult == SOCKET_ERROR) {
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

    while (true) {
        std::cout << "Please input your desired message:" << std::endl;
        std::getline(std::cin, message);
        const char *mssg = message.c_str();
        iresult = send(host_sock, mssg, (int)strlen(mssg), 0);

        if (iresult == SOCKET_ERROR) {
            std::cerr << "Failed to send..." << WSAGetLastError() << std::endl;
            closesocket(host_sock);
            WSACleanup();
            return 1;
        }
    }

    iresult = shutdown(host_sock, SD_SEND);
    if (iresult == SOCKET_ERROR) {
        std::cerr << "Failed to close properly..." << std::endl;
        closesocket(host_sock);
        host_sock = INVALID_SOCKET;
        return 1;
    }

    closesocket(host_sock);
    WSACleanup();

    return 0;
}