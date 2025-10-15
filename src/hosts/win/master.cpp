#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

int x, y;
size_t delimiter;

std::vector<char> buf(1024);

//Yes this list is sorted.
char keyList[] = {'\b', '\e', '\t', '\n', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ';', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']'};
int keyList_size = sizeof(keyList) / sizeof(keyList[0]); 

bool char_binary_search(char char_arr[], int arr_size, char target) {
    int low = 0;
    int high = arr_size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (char_arr[mid] == target) {
            return true; // Found the specified char
        } else if (char_arr[mid] < target) {
            low = mid + 1;
        } else { // Could also be defined as "arr[mid] > target"
            high = mid - 1;
        }
    }
    return false;
}

BOOL WINAPI break_handler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
            return TRUE;
        case CTRL_BREAK_EVENT:
            return TRUE;
        case CTRL_CLOSE_EVENT:
            return TRUE;
        case CTRL_LOGOFF_EVENT:
            return TRUE;
        case CTRL_SHUTDOWN_EVENT:
            return TRUE;
        default:
            return FALSE;
    }
}

void press_char_key(char mssg) {
    INPUT inp;

    inp.type = INPUT_KEYBOARD;
    inp.ki.wScan = 0;
    inp.ki.time = 0;
    inp.ki.dwExtraInfo = 0;

    inp.ki.wVk = VkKeyScan(mssg);
    inp.ki.dwFlags = 0;
    SendInput(1, &inp, sizeof(INPUT));

    inp.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &inp, sizeof(INPUT));
}

bool press_functional_key(char mssg) {
    INPUT inp;

    inp.type = INPUT_KEYBOARD;
    inp.ki.wScan = 0;
    inp.ki.time = 0;
    inp.ki.dwExtraInfo = 0;

    switch (mssg) {
        case 'c':
            inp.ki.wVk = VK_CAPITAL;
            break;
        case 'd':
            inp.ki.wVk = VK_DELETE;
            break;
        case 'v':
            inp.ki.wVk = VK_DOWN;
            break;
        case '<':
            inp.ki.wVk = VK_LEFT;
            break;
        case '>':
            inp.ki.wVk = VK_RIGHT;
            break;
        case '^':
            inp.ki.wVk = VK_UP;
            break;
        case 's':
            inp.ki.wVk = VK_SHIFT;
            break;
        default:
            return false;
    }

    inp.ki.dwFlags = 0;
    SendInput(1, &inp, sizeof(INPUT));

    inp.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &inp, sizeof(INPUT));

    return true;
}

void move_cursor(std::string mssg) {
    delimiter = mssg.find(',');
    if (delimiter != std::string::npos) {
        x = std::stoi(mssg.substr(0, delimiter));
        y = std::stoi(mssg.substr(delimiter + 1));
        SetCursorPos(x, y);
    }
}

void client_handler(SOCKET c_sock){
    do {
        ssize_t recv_mssg = recv(c_sock, buf.data(), buf.size(), 0);
        std::string mssg_str = (recv_mssg, buf.data());
        char mssg_char = mssg_str[0];
        if (char_binary_search(keyList, keyList_size, mssg_char)) {
            press_char_key(mssg_char);
        } else if (mssg_str.contains(",") && mssg_str.length() > 1) {
            move_cursor(mssg_str);
        } else {
            if (press_functional_key(mssg_char) == false) {      //"key" will not work here because of the first condition.
                std::cerr << "No matching key detected... " << std::endl;
            }
        }
    } while (c_sock > 0);
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

    while (true) {
        SOCKET client_sock = accept(host_sock, (SOCKADDR*)&client_addr, &client_addr_size);
        if (client_sock == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            closesocket(host_sock);
            WSACleanup();
            return 1;
        } else {
            std::thread client_thread(client_handler, client_sock);
            client_thread.detach();
        }
    }
    return 0;
}