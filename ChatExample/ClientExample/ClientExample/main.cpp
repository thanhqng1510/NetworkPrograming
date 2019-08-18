#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

const char* PORT = "1707";

int main() {
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    addrinfo* serv_info;
    int status = getaddrinfo("127.0.0.1", PORT, &hints, &serv_info);
    if (status != 0) {
        std::cout << "Error getaddrinfo\n";
        return 0;
    }
    
    int sockfd;
    addrinfo* p;
    for (p = serv_info; p != nullptr; p = p->ai_next) {
        sockfd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);
        if (sockfd == -1) {
            std::cout << "Error socket\n";
            continue;
        }
    
        if (connect(sockfd, serv_info->ai_addr, serv_info->ai_addrlen) == -1) {
            std::cout << "Error connect\n";
            continue;
        }
        
        break;
    }
    if (p == nullptr) {
        std::cout << "Error linked list\n";
        return 0;
    }
    
    freeaddrinfo(serv_info);

    while (true) {
        char msg[1025];
        memset(msg, 0, sizeof(msg));
        std::cout << "Enter: ";
        std::cin.getline(msg, 1025);
        int bytes_sent = static_cast<int>(send(sockfd, msg, strlen(msg), 0));
        if (bytes_sent == -1) {
            std::cout << "Error send\n";
            break;
        }
        else if (bytes_sent < strlen(msg)) {
            std::cout << "Not finish send\n";
            break;
        }
    }
    
    close(sockfd);
}
