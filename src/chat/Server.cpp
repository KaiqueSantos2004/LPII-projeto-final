#ifdef _WIN32
    // Windows
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib") 
#else
    // Linux/POSIX
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include "chat/Server.hpp" 
#include "chat/ClientHandler.hpp" 

#include <iostream>
#include <thread>
#include <vector>

// Construtor, inicializa o servidor em uma porta específica:
Server::Server(int port, const std::string& log_file) : logger(log_file) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        logger.log("Falha ao inicializar o Winsock.");
        std::cerr << "Falha ao inicializar o Winsock." << std::endl;
        exit(1);
    }
#endif
    //Criar o socket do servidor
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        logger.log("Erro ao criar o socket do servidor.");
        std::cerr << "Erro ao criar o socket do servidor." << std::endl;
        exit(1);
    }
    
    //Configurar o endereço do servidor
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //Associar o socket ao endereço e porta (bind)
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) 
    {
        logger.log("Erro no bind do socket.");
        std::cerr << "Erro no bind do socket." << std::endl;
        CLOSE_SOCKET(server_socket);
        exit(1);
    }

    //Colocar o socket em modo de escuta (listen)
    if (listen(server_socket, 10) < 0) 
    {
        logger.log("Erro ao colocar o socket em modo de escuta.");
        std::cerr << "Erro ao colocar o socket em modo de escuta." << std::endl;
        CLOSE_SOCKET(server_socket);
        exit(1);
    }

    std::cout << "Servidor escutando na porta " << port << "..." << std::endl;
    logger.log("Servidor iniciado e escutando na porta " + std::to_string(port));
}


// Destrutor, faz a limpeza e fecha o socket do servidor 
Server::~Server() 
{
    CLOSE_SOCKET(server_socket);
#ifdef _WIN32
    WSACleanup();
#endif
    std::cout << "Servidor finalizado." << std::endl;
}


void Server::run() {
    while (true) {
        //Aceitar uma nova conexão
        sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);

        if (client_socket < 0) 
        {
            logger.log("Erro ao aceitar nova conexão.");
            std::cerr << "Erro ao aceitar nova conexão." << std::endl;
            continue;
        }
        
        std::string client_ip = inet_ntoa(client_address.sin_addr);
        int client_port = ntohs(client_address.sin_port);
        std::cout << "Nova conexão de " << client_ip << ":" << client_port << std::endl;
        logger.log("Nova conexão aceita de " + client_ip + ":" + std::to_string(client_port));

        //Criar thread, passando o logger e o shared_state
        std::thread client_thread([client_socket, this]() 
        {
            ClientHandler handler(client_socket, this->shared_state, this->logger);
            handler.handle_client();
        });
        
        client_thread.detach();
    }
}