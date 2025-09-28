#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

class ClientHandler {
public:
    //recebe o socket do cliente conectado
    ClientHandler(int client_socket);

    void handle_client();

private:
    int client_socket;
};

#endif // CLIENT_HANDLER_HPP