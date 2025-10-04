#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

#include "chat/SharedState.hpp"
#include "utils/Logger.hpp"

class ClientHandler {
public:
    //recebe o socket do cliente conectado
    ClientHandler(int client_socket, SharedState& shared_state, Logger& logger);

    ~ClientHandler();

    void handle_client();

private:
    int client_socket;
    SharedState& shared_state;
    Logger&      logger;

#ifdef _WIN32
    #define CLOSE_SOCKET closesocket
#else
    #define CLOSE_SOCKET close
#endif   
};

#endif // CLIENT_HANDLER_HPP