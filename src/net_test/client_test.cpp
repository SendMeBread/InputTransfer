#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>
#include <iostream>
#include <unistd.h>

int main() {
    std::cout << "Please input your desired port" << std::endl;
    int port;
    std::cin >> port;

    std::cin.ignore();
    
    std::cout << "Please input your desired message" << std::endl;
    std::string message;
    std::getline(std::cin, message);

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
    group_address.sin_port = (port);
    group_address.sin_addr.s_addr = inet_addr("224.0.1.0");

    if (sendto(sockfd, message.c_str(), message.length(), 0, (struct sockaddr*)&group_address, sizeof(group_address)) < 0) {
        std::cerr << "sendto error" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Sent message: " << message << std::endl;

    close(sockfd);
    return 0;
}