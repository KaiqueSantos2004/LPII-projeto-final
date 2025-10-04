#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
public:
    // Prepara o cliente para conectar a um IP e porta
    Client(const std::string& server_ip, int port);

    ~Client();

    bool connect_to_server();

    void run();

private:
    void send_messages();

    void receive_messages();

    std::string server_ip;
    int         port;
    int         client_socket;

#ifdef _WIN32
    #define CLOSE_SOCKET closesocket
#else
    #define CLOSE_SOCKET close
#endif
};

#endif // CLIENT_HPP