#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

const char* PORT = "1707";
const int BACK_LOG = 20;

int main() {
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    
    addrinfo* serv_info;
    int status = getaddrinfo(nullptr, PORT, &hints, &serv_info);
    if (status != 0) {
        std::cout << "Error getaddrinfo\n";
        return 0;
    }
    
    int listen_sock = 0;
    addrinfo* p;
    for (p = serv_info; p != nullptr; p = p->ai_next) {
        listen_sock = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);
        if (listen_sock == -1) {
            std::cout << "Error socket\n";
            continue;
        }
    
        if (bind(listen_sock, serv_info->ai_addr, serv_info->ai_addrlen) == -1) {
            std::cout << "Error bind\n";
            continue;
        }
    
        if (listen(listen_sock, BACK_LOG) == -1) {
            std::cout << "Error listen\n";
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
        sockaddr_storage client;
        socklen_t client_size = sizeof(client);
        int new_sock = accept(listen_sock, (sockaddr*)&client, &client_size);
        
        char client_addr[INET6_ADDRSTRLEN];
        memset(client_addr, 0, INET6_ADDRSTRLEN);
        if (client.ss_family == AF_INET)
            std::cout << inet_ntop(AF_INET, &((sockaddr_in*)&client)->sin_addr, client_addr, INET6_ADDRSTRLEN) << " connected\n";
        else
            std::cout << inet_ntop(AF_INET6, &((sockaddr_in6*)&client)->sin6_addr, client_addr, INET6_ADDRSTRLEN) << " connected\n";
            
        while (true) {
            char received[1025];
            memset(received, 0, sizeof(received));
            int bytes_recv = static_cast<int>(recv(new_sock, received, 1025, 0));
            if (bytes_recv == -1) {
                std::cout << "Error recv\n";
                break;
            }
            else if (bytes_recv == 0) {
                std::cout << "Lost connection recv\n";
                break;
            }
            std::cout << "[Client]: " << received << "\n";
            
            char sent[1025];
            memset(sent, 0, sizeof(sent));
            std::cout << "[Server]: ";
            std::cin.getline(sent, 1025);
            int bytes_sent = static_cast<int>(send(new_sock, sent, strlen(sent), 0));
            if (bytes_sent == -1) {
                std::cout << "Error send\n";
                break;
            }
            else if (bytes_sent < strlen(sent))
                std::cout << "Not finish send\n";
        }
        
        close(new_sock);
    }
}
