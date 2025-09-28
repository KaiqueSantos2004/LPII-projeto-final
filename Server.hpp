#ifndef SERVER_HPP
#define SERVER_HPP

class Server {
public:
    //inicializa o servidor em uma porta específica
    Server(int port);

    void run();

private:
    int server_socket;
};

#endif // SERVER_HPP