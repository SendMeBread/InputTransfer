#include <X11/Xlib.h>
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <vector>

int port;
std::string ip;

std::ostringstream oss;

Display* display = XOpenDisplay(nullptr);

std::vector<char> buf(1024);

ssize_t recv_mssg;

ssize_t delimiter;
std::string coords;

std::string sub1, sub2;

int x, y;

int main() {
    if (display == nullptr) {
        return 1;
    }

    std::cout << "Please input your desired port:" << std::endl;
    std::cin >> port;

    std::cin.ignore();

    std::cout << "Please input your desired ip:" << std::endl;
    std::cin >> ip;

    std::cin.ignore();

    int sockfd;
    struct sockaddr_in address;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        std::cerr << "Error establishing socket." << std::endl;
        return 1;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    oss << "Joined " << ip << " on port " << port;
    std::string status = oss.str();
    std::cout << status << std::endl;

    std::vector<char> buffer(1024);
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    struct sockaddr_in c_address;
    socklen_t c_address_size = sizeof(c_address);

    bind(sockfd, (struct sockaddr*)&address, sizeof(address));  
    listen(sockfd, -1);

    while (true) {
        int sock_c = accept(sockfd, (struct sockaddr*)&c_address, &c_address_size);

        while (true) {
            recv_mssg = recv(sock_c, buf.data(), buf.size(), 0);
            if (recv_mssg < 0) {
                std::cerr << "recvfrom error" << std::endl;
                break;
            } else if (recv_mssg == 0) {
                std::cout << "Cursor client disconnected..." << std::endl;
                break;
            } else {
                coords = buf.data(), recv_mssg;
                delimiter = coords.find(',');

                if (delimiter != std::string::npos) {
                    sub1 = coords.substr(0, delimiter);
                    sub2 = coords.substr(delimiter + 1);

                    x = std::stoi(sub1);
                    y = std::stoi(sub2);
                }
            }
            XWarpPointer(display, None, DefaultRootWindow(display), 0, 0, 0, 0, x, y);
            XFlush(display);
        }
    }
    close(sockfd);
    XCloseDisplay(display);
    return 0;
}