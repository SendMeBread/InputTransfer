#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <string>

#include <windows.h>

void move_cursor(SOCKET c_sock) {
    std::vector<char> buf(1024);
    std::string stream_buffer = ""; // Accumulate data here

    while (true) {
        int recv_mssg = recv(c_sock, buf.data(), static_cast<int>(buf.size()), 0);
        if (recv_mssg < 0) {
            std::cerr << "recv error: " << WSAGetLastError() << std::endl;
            break;
        } else if (recv_mssg == 0) {
            std::cout << "A client disconnected..." << std::endl;
            break;
        }

        // Append newly received data to our stream accumulator
        stream_buffer.append(buf.data(), recv_mssg);

        // Process packets assuming they end with a delimiter like '\n'
        // Note: Ensure your client sends a '\n' at the end of every coordinate string!
        size_t newline_pos;
        while ((newline_pos = stream_buffer.find('\n')) != std::string::npos) {
            std::string packet = stream_buffer.substr(0, newline_pos);
            stream_buffer.erase(0, newline_pos + 1); // Remove processed packet

            size_t delimiter = packet.find(',');
            if (delimiter != std::string::npos) {
                std::string sub1 = packet.substr(0, delimiter);
                std::string sub2 = packet.substr(delimiter + 1);

                // Protect your code from throwing uncaught exceptions
                try {
                    int x = std::stoi(sub1);
                    int y = std::stoi(sub2);
                    SetCursorPos(x, y);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument error (not a number): " << e.what() << " | Data received: " << packet << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Value out of integer range: " << e.what() << std::endl;
                }
            }
        }
    }
    shutdown(c_sock, SD_SEND);
    closesocket(c_sock);
    return;
}

int main(int argc, char* argv[]) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) < 0) {
        std::cerr << "WSAStartup error..." << std::endl;
    }

    std::string ip = argv[1];
    std::string port_str = argv[2];
    int port = std::stoi(port_str);

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
        WSACleanup();
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