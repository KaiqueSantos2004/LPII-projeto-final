#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

#include "chat/SharedState.hpp"
#include "utils/Logger.hpp"
#include <string>
#include <vector>

class ClientHandler {
public:
    //recebe o socket do cliente conectado
    ClientHandler(int client_socket, SharedState& shared_state, Logger& logger, const std::vector<std::string>& forbidden_words);

    ~ClientHandler();

    void handle_client();

private:
    std::string filtrar_mensagem(std::string msg);
    int client_socket;
    SharedState& shared_state;
    Logger&      logger;
    const std::vector<std::string>  & forbidden_words; 


#ifdef _WIN32
    #define CLOSE_SOCKET closesocket
#else
    #define CLOSE_SOCKET close
#endif   
};

#endif // CLIENT_HANDLER_HPP