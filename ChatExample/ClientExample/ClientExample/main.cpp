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
    
    int sockfd = 0;
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
        char sent[1025];
        memset(sent, 0, sizeof(sent));
        std::cout << "Enter: ";
        std::cin.getline(sent, 1025);
        int bytes_sent = static_cast<int>(send(sockfd, sent, strlen(sent), 0));
        if (bytes_sent == -1) {
            std::cout << "Error send\n";
            break;
        }
        else if (bytes_sent < strlen(sent))
            std::cout << "Not finish send\n";
                
        char received[1025];
        memset(received, 0, sizeof(received));
        int bytes_recv = static_cast<int>(recv(sockfd, received, 1025, 0));
        if (bytes_recv == -1) {
            std::cout << "Error recv\n";
            break;
        }
        else if (bytes_recv == 0) {
            std::cout << "Lost connection recv\n";
            break;
        }
        std::cout << "Received: " << received << "\n";
    }
    
    close(sockfd);
}
