#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

int x, y;
size_t delimiter;

void move_cursor(std::string mssg) {
    delimiter = mssg.find(',');
    if (delimiter != std::string::npos) {
        x = std::stoi(mssg.substr(0, delimiter));
        y = std::stoi(mssg.substr(delimiter + 1));
        SetCursorPos(x, y);
    }
}

void press_A(std::string mssg) {
    if (mssg.contains("A")) {
        INPUT inp;

        inp.type = INPUT_KEYBOARD;
        inp.ki.wScan = 0;
        inp.ki.time = 0;
        inp.ki.dwExtraInfo = 0;

        inp.ki.wVk = VkKeyScan('A');
        inp.ki.dwFlags = 0;
        SendInput(1, &inp, sizeof(INPUT));

        inp.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &inp, sizeof(INPUT));
    }
}

void press_B(std::string mssg) {
    if (mssg == "B") {
        INPUT inp;

        inp.type = INPUT_KEYBOARD;
        inp.ki.wScan = 0;
        inp.ki.time = 0;
        inp.ki.dwExtraInfo = 0;

        inp.ki.wVk = VkKeyScan('B');
        inp.ki.dwFlags = 0;
        SendInput(1, &inp, sizeof(INPUT));

        inp.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &inp, sizeof(INPUT));
    }
}

void press_C(std::string mssg) {
    if (mssg == "C") {
        INPUT inp;

        inp.type = INPUT_KEYBOARD;
        inp.ki.wScan = 0;
        inp.ki.time = 0;
        inp.ki.dwExtraInfo = 0;

        inp.ki.wVk = VkKeyScan('C');
        inp.ki.dwFlags = 0;
        SendInput(1, &inp, sizeof(INPUT));

        inp.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &inp, sizeof(INPUT));
    }
}

void press_D(std::string mssg) {
    if (mssg == "D") {
        INPUT inp;

        inp.type = INPUT_KEYBOARD;
        inp.ki.wScan = 0;
        inp.ki.time = 0;
        inp.ki.dwExtraInfo = 0;

        inp.ki.wVk = VkKeyScan('D');
        inp.ki.dwFlags = 0;
        SendInput(1, &inp, sizeof(INPUT));

        inp.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &inp, sizeof(INPUT));
    }
}

void press_E(std::string mssg) {
    if (mssg == "E") {
        INPUT inp;

        inp.type = INPUT_KEYBOARD;
        inp.ki.wScan = 0;
        inp.ki.time = 0;
        inp.ki.dwExtraInfo = 0;

        inp.ki.wVk = VkKeyScan('E');
        inp.ki.dwFlags = 0;
        SendInput(1, &inp, sizeof(INPUT));

        inp.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &inp, sizeof(INPUT));
    }
}

int main(int argc, char* argv[]) {
    WSAData wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) < 0) {
        std::cerr << "WSAStartup error..." << std::endl;
    }

    int port = std::stoi(argv[2]);
    std::string ip = argv[1];

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

    ssize_t recv_mssg;
    std::vector<char> buf(1024);

    SOCKET client_sock = INVALID_SOCKET;

    while (true) {
        client_sock = accept(host_sock, (SOCKADDR*)&client_addr, &client_addr_size);
        if (client_sock == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            closesocket(host_sock);
            WSACleanup();
            return 1;
        } else {
            recv_mssg = recv(client_sock, buf.data(), buf.size(), 0);
            if (recv_mssg < 0) {
                std::cerr << "recvfrom error" << std::endl;
                break;
            } else if (recv_mssg == 0) {
                std::cout << "A client disconnected..." << std::endl;
                break;
            } else {
                std::string input = (recv_mssg, buf.data());
                //Mouse threads
                std::thread pointer_thread(move_cursor, input);

                pointer_thread.detach();

                //Alphanumeric threads
                std::thread a_thread(press_A, input);
                std::thread b_thread(press_B, input);
                std::thread c_thread(press_C, input);
                std::thread d_thread(press_D, input);
                std::thread e_thread(press_E, input);

                a_thread.detach();
                b_thread.detach();
                c_thread.detach();
                d_thread.detach();
                e_thread.detach();
            }
        }
    }
}