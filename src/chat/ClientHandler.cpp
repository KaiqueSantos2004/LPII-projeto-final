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
    shared_state.add_client(client_socket);

    logger.log("Enviando histórico de mensagens para o cliente " + std::to_string(client_socket));
    std::vector<std::string> history = shared_state.get_history();
    for (const auto& old_message : history) {
        std::string history_msg = "[Histórico] " + old_message + "\n";
        send(client_socket, history_msg.c_str(), history_msg.length(), 0);
    }

    char buffer[4096];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {
            std::cout << "Cliente desconectado." << std::endl;
            logger.log("Cliente desconectado: " + std::to_string(client_socket));
            break;
        }
        std::string message(buffer, bytes_received);
        std::stringstream formatted_message_ss;
        formatted_message_ss << "Cliente " << client_socket << ": " << message;
        
        std::string formatted_message = formatted_message_ss.str(); 
        logger.log(formatted_message);

        shared_state.add_message(formatted_message);

        std::string broadcast_msg = formatted_message + "\n";

        std::vector<int> all_clients = shared_state.get_all_clients();
        for (int other_client_socket : all_clients) {
            if (other_client_socket != client_socket) {
                send(other_client_socket, broadcast_msg.c_str(), broadcast_msg.length(), 0);
            }
        }
    }

    shared_state.remove_client(client_socket);
}