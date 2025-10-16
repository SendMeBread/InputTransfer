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

char keyFuncList[] = { ' ', '<', '>', '^', 'a', 'b', 'c','d', 'e', 'i', 'n', 's', 't', 'v', 'w', 'x' };
int VkList[] = { VK_SPACE, VK_ESCAPE, VK_LEFT, VK_RIGHT, VK_UP, VK_MENU, VK_BACK, VK_CAPITAL, VK_DELETE, VK_INSERT, VK_RETURN, VK_SHIFT, VK_TAB, VK_DOWN, VK_LWIN, VK_CONTROL };

int find_char_arr_size(char *arr) {
    return sizeof(arr) / sizeof(arr[0]);
}

std::map<int, char> def_keyCodeMap(int *codes, char *keys) {
    std::map<int, char> map;
    for ( int i = 0; i < find_char_arr_size(keys); i++ ) {
        map[codes[i]] = keys[i]; 
    }
    return map;
}

std::map<int, char> keyMap = def_keyCodeMap(VkList, keyFuncList);

bool char_binary_search(char *char_arr, char target) {
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

void press_functional_key(char mssg) {
    INPUT inp[2];

    inp[0].type, inp[1].type = INPUT_KEYBOARD;
    inp[0].ki.wScan, inp[1].ki.wScan = 0;
    inp[0].ki.time, inp[1].ki.time = 0;
    inp[0].ki.dwExtraInfo, inp[1].ki.dwExtraInfo = 0;

    for (const auto& key_pair : keyMap) {
        if (mssg == key_pair.second) {
            inp[0].ki.wVk, inp[2].ki.wVk = key_pair.first;
        }
    }

    inp[0].ki.dwFlags = 0;
    inp[1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(2, inp, sizeof(INPUT));
}

void move_cursor(std::string coords) {
    delimiter = coords.find(',');
    if (delimiter != std::string::npos) {
        x = std::stoi(coords.substr(0, delimiter));
        y = std::stoi(coords.substr(delimiter + 1));
        SetCursorPos(x, y);
    }
}

bool sim_scroll_wheel(std::string factor, char definer) {
    try {
        factor.erase(std::remove(factor.begin(), factor.end(), definer), factor.end());
        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, WHEEL_DELTA * std::stoi(factor), 0);
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

void click_mouse_button(char button) {
    INPUT Linp[2];
    INPUT Rinp[2];
    INPUT Minp[2];

    Linp[0].type, Linp[1].type, Rinp[0].type, Rinp[1].type, Minp[0].type, Minp[1].type = INPUT_MOUSE;
    Linp[0].mi.dwFlags, Linp[1].mi.dwFlags, Rinp[0].mi.dwFlags, Rinp[1].mi.dwFlags, Minp[0].mi.dwFlags, Minp[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP, MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP;

    switch (button) {
        case 'l':
            SendInput(2, Linp, sizeof(INPUT));
        case 'r':
            SendInput(2, Rinp, sizeof(INPUT));
        case 'm':
            SendInput(2, Minp, sizeof(INPUT));
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
        } else if (mssg_str.contains("mb") && mssg_str.length() > 2) {
            click_mouse_button(mssg_str[2]);
        } else if (!sim_scroll_wheel(mssg_str, mssg_char)) {
            press_functional_key(mssg_char);
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