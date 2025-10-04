#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils/Logger.hpp"
#include "chat/SharedState.hpp"
#include <string>

class Server {
public:
    //inicializa o servidor em uma porta específica
    Server(int port, const std::string& log_file);

    ~Server();

    void run();

private:
    int server_socket;
    Logger      logger;
    SharedState shared_state;
    
    #ifdef _WIN32
    #define CLOSE_SOCKET closesocket
    #else
    #define CLOSE_SOCKET close
    #endif
};

#endif // SERVER_HPP