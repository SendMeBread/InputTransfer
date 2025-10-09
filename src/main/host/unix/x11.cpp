#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <X11/Xlib.h>
std::vector<std::string> str_split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream split_ss(str);
    std::string token;

    while (std::getline(split_ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
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

    std::cin.ignore();

    std::cout << "Please input your desired ip" << std::endl;
    std::string ip;
    std::cin >> ip;


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

    std::ostringstream oss_ip;

    oss_ip << "Joined " << ip << " on port " << port;
    std::string status = oss_ip.str();
    std::cout << status << std::endl;

    std::vector<char> buffer(1024);
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    struct sockaddr_in c_address;
    socklen_t c_address_size = sizeof(c_address);

    bind(sockfd, (struct sockaddr*)&address, sizeof(address));  
    listen(sockfd, 20);

    int sock_c = accept(sockfd, (struct sockaddr*)&c_address, &c_address_size);
    
    std::stringstream ss;

    while (true) {
        ssize_t bytes_received = recv(sock_c, buffer.data(), buffer.size(), 0);
        if (bytes_received > 0) {
            std::cout << "Message: " << std::endl;
            
            ss.write(buffer.data(), bytes_received);
            std::string mssg_str = ss.str();
            mssg_str.erase(std::remove(' '), mssg_str.end());
            mssg_str.erase(std::remove('('), mssg_str.end());
            mssg_str.erase(std::remove(')'), mssg_str.end());

            std::vector<std::string> str_arr = str_split(mssg_str, char(','));
            std::cout.write(buffer.data(), bytes_received) << std::endl;
        } else if (bytes_received == 0) {
            std::cout << "Connection closed by peer..." << std::endl;
            break;
        } else {
            perror("failed recv");
            break;
        }   
    }


    XWarpPointer(display, None, root, 0,0,0,0, x, y);
    XFlush(display);
    XCloseDisplay(display);

    return 0;
}
