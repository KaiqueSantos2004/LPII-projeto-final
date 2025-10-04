#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
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

//Construtor: Prepara o cliente para conectar a um IP e porta 
Client::Client(const std::string& ip, int p) : server_ip(ip), port(p), client_socket(-1){
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        std::cerr << "Falha ao inicializar o Winsock." << std::endl;
        exit(1);
    }
#endif
}

//Destrutor: fecha o socket e limpa o Winsock caso esteja em windows
Client::~Client(){
    if (client_socket != -1)
    {
        CLOSE_SOCKET(client_socket);
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

//conecta ao servidor
bool Client::connect_to_server(){
    //cria o socket do cliente
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) 
    {
        std::cerr << "Erro ao criar o socket do cliente." << std::endl;
        return false;
    }

    //configura o endereço do servidor ao qual vamos nos conectar
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr);//conversor de string para in_addr

    //tenta conectar ao servidor
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) 
    {
        std::cerr << "Falha ao conectar ao servidor." << std::endl;
        CLOSE_SOCKET(client_socket);
        client_socket = -1;
        return false;
    }

    std::cout << "Conectado ao servidor!" << std::endl;
    return true;
}

void Client::run(){
    if (client_socket == -1)
    {
        std::cerr << "Cliente não está conectado." << std::endl;
        return;
    }

    //Tem duas threads: Uma para enviar mensagens e outra para recebê-las
    std::thread sender_thread(&Client::send_messages, this);
    std::thread receiver_thread(&Client::receive_messages, this);

    sender_thread.join();
    receiver_thread.join();
}

//envia a mensagem para o servidor
void Client::send_messages(){
    std::string line;
    while (std::getline(std::cin, line))
    {   
        if (line == "exit") {
            break;
        }
        
        send(client_socket, line.c_str(), line.length(), 0);
    }
}

//recebe mensagens do servidor
void Client::receive_messages()
{
    char buffer[4096];
    while (true) {
        //Zera o buffer antes de receber os dados
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) 
        {
            std::cout << "Desconectado do servidor." << std::endl;
            break;
        }

        std::cout << "Recebido: " << std::string(buffer, bytes_received) << std::endl;
    }
}
