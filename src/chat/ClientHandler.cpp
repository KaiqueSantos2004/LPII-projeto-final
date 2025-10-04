#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <unistd.h>
#endif

#include "chat/ClientHandler.hpp" 
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>


//construtor: armazena as referências e o socket do cliente
ClientHandler::ClientHandler(int socket, SharedState& state, Logger& log)
    : client_socket(socket), shared_state(state), logger(log){
}

//destrutor: garante que o socket seja fechado
ClientHandler::~ClientHandler(){
    CLOSE_SOCKET(client_socket);
}

void ClientHandler::handle_client() {
    //adiciona o  cliente à lista de clientes compartilhada
    shared_state.add_client(client_socket);

    char buffer[4096];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {
            std::cout << "Cliente desconectado." << std::endl;
            logger.log("Cliente desconectado.");
            break; 
        }

        std::string message(buffer, bytes_received);
        std::stringstream ss;
        ss << "Mensagem recebida do cliente " << client_socket << ": " << message;
        logger.log(ss.str());

        // faz o broadcast da mensagem para todos os outros clientes
        std::vector<int> all_clients = shared_state.get_all_clients();
        for (int other_client_socket : all_clients) {
            if (other_client_socket != client_socket) {
                send(other_client_socket, message.c_str(), message.length(), 0);
            }
        }
    }

    //limpeza: remove o cliente da lista compartilhada ao sair do loop
    shared_state.remove_client(client_socket);
}