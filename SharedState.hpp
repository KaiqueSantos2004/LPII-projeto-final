#ifndef SHARED_STATE_HPP
#define SHARED_STATE_HPP

#include <vector>
#include <mutex>

//Gerencia os dados compartilhados entre as threads de forma segura. Adicionando ou retirando clientes(sockets) da lista compartilhada.
class SharedState {
public:
    SharedState();

    void add_client(int client_socket);

    void remove_client(int client_socket);

    std::vector<int> get_all_clients();

private:
    std::vector<int> clients; 
    std::mutex mtx;          
};

#endif // SHARED_STATE_HPP