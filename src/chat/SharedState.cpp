
#include "chat/SharedState.hpp" 
#include <algorithm>


SharedState::SharedState() {
}


//rdiciona um novo socket de cliente ao vetor de forma thread-safe
void SharedState::add_client(int client_socket) 
{
    std::lock_guard<std::mutex> lock(mtx);
    clients.push_back(client_socket);
}

//remove um socket de cliente do vetor de forma thread-safe
void SharedState::remove_client(int client_socket) 
{
    std::lock_guard<std::mutex> lock(mtx);
    clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
}

//retorna uma cópia do vetor de sockets de clientes de forma thread-safe
std::vector<int> SharedState::get_all_clients() 
{
    std::lock_guard<std::mutex> lock(mtx);
    return clients;
}



