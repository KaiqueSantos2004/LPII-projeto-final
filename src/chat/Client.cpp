#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include "chat/Client.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <limits> // Necessário para a limpeza do cin

Client::Client(const std::string& ip, int p) 
    : server_ip(ip), port(p), client_socket(-1), running(false) { 
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Falha ao inicializar o Winsock." << std::endl;
        exit(1);
    }
#endif
}

// Destrutor: fecha o socket e limpa o Winsock caso esteja em windows
Client::~Client() {
    if (client_socket != -1) {
        CLOSE_SOCKET(client_socket);
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

// conecta ao servidor
bool Client::connect_to_server() {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        return false;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr);

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        CLOSE_SOCKET(client_socket);
        client_socket = -1;
        return false;
    }

    std::cout << "Conectado ao servidor! Digite 'exit' para sair." << std::endl;
    running = true; // Seta para true apenas após conectar
    return true;
}


void Client::run() {
    if (!running) {
        std::cerr << "Cliente não está conectado." << std::endl;
        return;
    }

    std::thread sender_thread(&Client::send_messages, this);
    std::thread receiver_thread(&Client::receive_messages, this);

    if (sender_thread.joinable()) sender_thread.join();
    if (receiver_thread.joinable()) receiver_thread.join();
}

// envia a mensagem para o servidor
void Client::send_messages() {
    std::string line;
    std::cout << ">> ";
    std::cout.flush(); 

    while (running && std::getline(std::cin, line)) {
        if (!running) break; // Checagem extra

        if (line == "exit") {
            running = false;
            #ifdef _WIN32
                shutdown(client_socket, SD_BOTH);
            #else
                shutdown(client_socket, SHUT_RDWR);
            #endif
            break;
        }

        if (send(client_socket, line.c_str(), line.length(), 0) < 0) {
            if (running) {
                std::cerr << "Falha ao enviar mensagem." << std::endl;
            }
            break;
        }
                if (running) {
            std::cout << ">> ";
            std::cout.flush();
        }
    }
}

// recebe mensagens do servidor
void Client::receive_messages() {
    char buffer[4096];
    while (running) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        
        if (bytes_received <= 0) {
            if (running) {
                std::cout << "Desconectado do servidor." << std::endl;
            }
            running = false;
            break;
        }

        std::cout << "Recebido: " << std::string(buffer, bytes_received) << std::endl;
    }
}