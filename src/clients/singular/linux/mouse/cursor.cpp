#include <X11/Xlib.h>
#include <iostream>

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>
#include <unistd.h>
#include <thread>
#include <vector>

std::string coords;

int main() {
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cerr << "Unable to open X display" << std::endl;
        return 1;
    } else {
        std::cout << "X display opened successfully" << std::endl;
    }

    Window root = DefaultRootWindow(display);

    int x, y;
    Window returned_root, returned_child;
    int win_x, win_y;
    unsigned int mask_return;

    std::cout << "Please input your desired port:" << std::endl;
    int port;
    std::cin >> port;

    std::cin.ignore();

    std::cout << "Please input your desired ip:" << std::endl;
    std::string ip_str;
    std::cin >> ip_str;
    std::vector<char> ip(ip_str.begin(), ip_str.end());
    ip.push_back('\0');
    char* ip_arr = ip.data();

    std::cin.ignore();

    int sockfd;
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip_arr, &(address.sin_addr));
    

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error establishing socket." << std::endl;
        return 1;
    }

    if (connect(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Connection failed..." << std::endl;
        close(sockfd);
        return 1;
    };

    while (true) {
        if (XQueryPointer(display, root, &returned_root, &returned_child, &x, &y, &win_x, &win_y, &mask_return)) {
            coords = std::to_string(x) + "," + std::to_string(y);
            if (send(sockfd, coords.c_str(), coords.length(), 0) < 0) {
                std::cerr << "sendto error" << std::endl;
                close(sockfd);
                return 1;
            }
            //std::cout << "Cursor Position: (" << x << ", " << y << ")" << std::endl;
        } else {
            std::cerr << "Failed to query pointer, retrying..." << std::endl;
            break;

        }
    }

    XCloseDisplay(display);

    return 0;
}