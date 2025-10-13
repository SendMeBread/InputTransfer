#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <vector>
#include <string>

#include <windows.h>

WSADATA wsaData;

int port;
std::string ip;

std::vector<char> buf(1024);

std::string coords;

std::string sub1;
std::string sub2;

int x;
int y;

size_t delimiter;

void move_cursor(SOCKET c_sock) {
    while (true) {
        ssize_t recv_mssg = recv(c_sock, buf.data(), buf.size(), 0);
        if (recv_mssg < 0) {
            std::cerr << "recvfrom error" << std::endl;
            break;
        } else if (recv_mssg == 0) {
            std::cout << "A client disconnected..." << std::endl;
            break;
        } else {
            coords = buf.data(), recv_mssg;
            delimiter = coords.find(',');

            if (delimiter != std::string::npos) {
                sub1 = coords.substr(0, delimiter);
                sub2 = coords.substr(delimiter + 1);

                x = std::stoi(sub1);
                y = std::stoi(sub2);

                SetCursorPos(x, y);
            }
        }
    }
    shutdown(c_sock, SD_SEND);
    closesocket(c_sock);
    return;
}

int main() {
    int iresult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iresult < 0) {
        std::cerr << "WSAStartup error..." << std::endl;
    }

    std::cout << "Please input your desired port:" << std::endl;
    std::cin >> port;

    std::cout << "Please input your desired ip:" << std::endl;
    std::cin >> ip;

    SOCKET host_sock = INVALID_SOCKET;
    host_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (host_sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(port);

    if (bind(host_sock, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "Binding failed: " << WSAGetLastError() << std::endl;
        closesocket(host_sock);
        WSACleanup();
    }

    if (listen(host_sock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Socket error: " << WSAGetLastError() << std::endl;
        closesocket(host_sock);
        WSACleanup;
    }

    sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);
    SOCKET client_sock = INVALID_SOCKET;

    while (true) {
        client_sock = accept(host_sock, (SOCKADDR*)&client_addr, &client_addr_size);
        if (client_sock == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            closesocket(host_sock);
            WSACleanup();
            return 1;
        }
        std::thread c_thread(move_cursor, client_sock);
        c_thread.detach();
    }
    closesocket(host_sock);

    WSACleanup();

    return 0;
}