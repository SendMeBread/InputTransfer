#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <vector>
int main() {
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

    std::ostringstream oss;
    oss << "Joined " << ip << " on port " << port;
    std::string status = oss.str();
    std::cout << status << std::endl;

    std::vector<char> buffer(1024);
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    struct sockaddr_in c_address;
    socklen_t c_address_size = sizeof(c_address);

    bind(sockfd, (struct sockaddr*)&address, sizeof(address));  
    listen(sockfd, 20);

    int sock_c = accept(sockfd, (struct sockaddr*)&c_address, &c_address_size);

    while (true) {
        ssize_t bytes_received = recv(sock_c, buffer.data(), buffer.size(), 0);

        if (bytes_received > 0) {
            std::cout << "Message: " << std::endl;
            std::cout.write(buffer.data(), bytes_received) << std::endl;
        } else if (bytes_received == 0) {
            std::cout << "Connection closed by peer..." << std::endl;
            break;
        } else {
            perror("failed recv");
            break;
        }   
    }
    close(sock_c);
    return 0;
}