#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <vector>

int main() {
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
        std::cout << "Please input your desired message:" << std::endl;
        std::string message;
        std::getline(std::cin, message);
        if (message == "" || message == "quit" || message == "exit") {
            std::cout << "Exiting..." << std::endl;
            break;
        }
        if (send(sockfd, message.c_str(), message.length(), 0) < 0) {
            std::cerr << "sendto error" << std::endl;
            close(sockfd);
            return 1;
        } else {
            std::cout << "Sent message: " << message << std::endl;
        }
    }

    close(sockfd);
    return 0;
}