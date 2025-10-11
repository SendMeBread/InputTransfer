#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

void print_message(SOCKET c_sock, int ir) {
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
            std::cout.write(buf.data(), recv_mssg) <<std::endl;
        }
    }
    ir = shutdown(c_sock, SD_SEND);
    closesocket(c_sock);
    return;
}

int main() {
    WSAData wsaData;
    int iresult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iresult != 0) {
        std::cerr << "WSAStartup error..." << std::endl;
    }

    std::string port;
    std::getline(std::cin, port);

    struct addrinfo *result = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iresult = getaddrinfo(NULL, port.c_str(), &hints, &result);

    SOCKET host_sock = INVALID_SOCKET;
    host_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    iresult = bind(host_sock, result->ai_addr, (int)result->ai_addrlen);

    freeaddrinfo(result);

    iresult = listen(host_sock, SOMAXCONN);

    while (true) {
        SOCKET client_sock = INVALID_SOCKET;
        client_sock = accept(host_sock, NULL, NULL);
        std::thread c_thread(print_message, client_sock, iresult);
        c_thread.detach();
    }
    WSACleanup();
}