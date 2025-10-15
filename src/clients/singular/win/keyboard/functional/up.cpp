#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <iostream>
#include <chrono>

std::string message = "^";

int main(int argc, char* argv[]) {
    WSADATA wsaData;
    int iresult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iresult < 0) {
        std::cerr << "WSAStartup error..." << std::endl;
    }

    struct addrinfo *result = NULL, *ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));

    std::string ip = argv[1];
    std::string port = argv[2];

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
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            const char* mssg = message.c_str();
                if (send(host_sock, mssg, (int)strlen(mssg), 0) == SOCKET_ERROR) {
                    std::cerr << "Failed to send..." << WSAGetLastError() << std::endl;
                    closesocket(host_sock);
                    WSACleanup();
                    return 1;
            }
        }
        Sleep(5);
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