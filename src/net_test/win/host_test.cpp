#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

void print_message(SOCKET c_sock) {
    std::vector<char> buf(1024);
    while (true) {
        ssize_t recv_mssg = recv(c_sock, buf.data(), buf.size(), 0);
        if (recv_mssg < 0) {
            std::cerr << "recvfrom error" << std::endl;
            break;
        } else if (recv_mssg == 0) {
            std::cout << "A client disconnected..." << std::endl;
            break;
        } else {
            std::cout << "Received message: " << std::endl;
            std::cout.write(buf.data(), recv_mssg) << std::endl;
        }
    }
    shutdown(c_sock, SD_SEND);
    closesocket(c_sock);
    return;
}

int main() {
    WSAData wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) < 0) {
        std::cerr << "WSAStartup error..." << std::endl;
    }

    int port;
    std::string ip;

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
        } else {
            std::thread c_thread(print_message, client_sock);
            c_thread.detach();
        }
    }

    closesocket(host_sock);

    WSACleanup();

    return 0;
}