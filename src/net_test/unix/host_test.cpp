#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <thread>
void print_message(int client_sock) {
    std::vector<char> buf(1024);
    while (true) {
        ssize_t recv_mssg = recv(client_sock, buf.data(), buf.size(), 0);
        if (recv_mssg < 0) {
            std::cerr << "recvfrom error" << std::endl;
            break;
        } else if (recv_mssg == 0) {
            std::cout << "A client disconnected..." << std::endl;
            break;
        } else {
            std::cout << "Received message: " << std::endl;
            std::cout.write(buf.data(), recv_mssg) <<std::endl;
        }
    }
    close(client_sock);
    return;
}
int main() {
    std::cout << "Please input your desired port:" << std::endl;
    int port;
    std::cin >> port;

    std::cin.ignore();

    std::cout << "Please input your desired ip:" << std::endl;
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
    listen(sockfd, -1);


    while (true) {
        int sock_c = accept(sockfd, (struct sockaddr*)&c_address, &c_address_size);
        std::thread c_handler(print_message, sock_c);
        c_handler.detach();
    }
    close(sockfd);
    return 0;
}