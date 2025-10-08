#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <X11/Xlib.h>

int main() {
    Display* display = XOpenDisplay(nullptr);

    if (display == nullptr) {
        std::cerr << "Error: Could not open X display." << std::endl;
        return 1;
    }

    Window root = DefaultRootWindow(display);

    int x;
    int y;

    std::cout << "Please input your desired port" << std::endl;
    int port;
    std::cin >> port;

    int sockfd;
    struct sockaddr_in group_address;
    struct ip_mreq mreq;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error establishing socket." << std::endl;
        return 1;
    }

    memset(&group_address, 0, sizeof(group_address));
    group_address.sin_family = AF_INET;
    group_address.sin_port = (5000);
    group_address.sin_addr.s_addr = inet_addr("224.0.1.0");

    if (bind(sockfd, (struct sockaddr*)&group_address, sizeof(group_address)) < 0) {
        std::cerr << "Error binding socket." << std::endl;
        close(sockfd);
        return 1;
    }

    mreq.imr_multiaddr.s_addr = inet_addr("224.0.1.0");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        std::cerr << "Error joining multicast group" << std::endl;
        close(sockfd);
        return 1;
    }

    std::ostringstream oss;
    oss << "Joined group 244.0.1.0 on port " << port;
    std::string status = oss.str();
    std::cout << status << std::endl;

    char buffer[1024];
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    while (true) {
        int bytes_receieved = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&sender_addr,  &sender_addr_len);
        if (bytes_receieved < 0) {
            std::cerr << "Error!" << std::endl;
            close(sockfd);
            return 1;
        } else if (bytes_receieved == 0) {
            std::cerr << "No message..." << std::endl;
            close(sockfd);
            return 1;
        } else {
            std::string message(buffer, bytes_receieved);
            if (message.find(",") != std::string::npos) {
                std::stringstream ss(message);
                std::string segment;

                if (std::getline(ss, segment, ',')) {
                    try {
                        x = std::stoi(segment);
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Invalid argument for first integer: " << e.what() << std::endl;
                        return 1; // Indicate an error
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Out of range for first integer: " << e.what() << std::endl;
                        return 1; // Indicate an error
                    }
                } else {
                    std::cerr << "Could not find the first integer segment." << std::endl;
                    return 1; // Indicate an error
                }
                if (std::getline(ss, segment)) {
                    try {
                        y = std::stoi(segment);
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Invalid argument for second integer: " << e.what() << std::endl;
                        return 1; // Indicate an error
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Out of range for second integer: " << e.what() << std::endl;
                        return 1; // Indicate an error
                    }
                } else {
                    std::cerr << "Could not find the second integer segment." << std::endl;
                    return 1; // Indicate an error
                }
            }
            std::cout << "Message: " << message << std::endl;
        }
    }
    close(sockfd);
    return 0;

    XWarpPointer(display, None, root, 0,0,0,0, x, y);

    XFlush(display);
    
    XCloseDisplay(display);
    return 0;
}
