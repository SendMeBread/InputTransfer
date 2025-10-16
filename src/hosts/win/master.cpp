#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <windows.h>

int x, y;
size_t delimiter;

std::vector<char> buf(1024);

char keyCharList[] = { '\'', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ';', '=', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '`' };

char keyFuncList[] = { '\b', '\t', '\n', '\e', ' ', '<', '>', '^', 'a', 'c', 'd', 'i', 's', 'v', 'w', 'x' };
int VkList[] = { VK_BACK, VK_TAB, VK_RETURN, VK_SPACE, VK_ESCAPE, VK_LEFT, VK_RIGHT, VK_UP, VK_MENU, VK_CAPITAL, VK_DELETE, VK_INSERT, VK_SHIFT, VK_DOWN, VK_LWIN, VK_CONTROL };

int find_char_arr_size(char *arr) {
    return sizeof(arr) / sizeof(arr[0]);
}

std::map<int, char> def_keyCodeMap(int codes[], char keys[]) {
    std::map<int, char> map;
    for ( int i = 0; i < find_char_arr_size(keys); i++ ) {
        map[codes[i]] = keys[i]; 
    }
    return map;
}

std::map<int, char> keyMap = def_keyCodeMap(VkList, keyFuncList);

bool char_binary_search(char char_arr[], char target) {
    int low = 0;
    int high = find_char_arr_size(char_arr) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (char_arr[mid] == target) {
            return true;
        } else if (char_arr[mid] < target) {
            low = mid + 1;
        } else {
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

    for (const auto& key_pair : keyMap) {
        if (mssg == key_pair.second) {
            inp.ki.wVk = key_pair.first;
        }
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
        if (char_binary_search(keyCharList, mssg_char)) {
            press_char_key(mssg_char);
        } else if (mssg_str.contains(",") && mssg_str.length() > 1) {
            move_cursor(mssg_str);
        } else if (press_functional_key(mssg_char) == false) {
            std::cerr << "No matching key detected... " << std::endl;
        }
    } while (c_sock > 0);
}

int main() {
    WSAData wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) < 0) {
        std::cerr << "WSAStartup error..." << std::endl;
        return 1;
    }

    std::string ip;
    int port;

    std::cout << "Please input your desired ip:" << std::endl;
    std::cin >> ip;

    std::cout << "Please input your desired port:" << std::endl;
    std::cin >> port;

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
        return 1;
    }

    if (listen(host_sock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Socket error: " << WSAGetLastError() << std::endl;
        closesocket(host_sock);
        WSACleanup;
        return 1;
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