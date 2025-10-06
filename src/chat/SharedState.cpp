#include "chat/SharedState.hpp"
#include <algorithm>

SharedState::SharedState() {
}

void SharedState::add_client(int client_socket) {
    std::lock_guard<std::mutex> lock(mtx);
    clients.push_back(client_socket);
}

void SharedState::remove_client(int client_socket) {
    std::lock_guard<std::mutex> lock(mtx);
    clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
}

std::vector<int> SharedState::get_all_clients() {
    std::lock_guard<std::mutex> lock(mtx);
    return clients;
}

// Implementação da função de adicionar mensagem
void SharedState::add_message(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    
    message_history.push_back(message);
    
    if (message_history.size() > MAX_HISTORY_SIZE) {
        message_history.pop_front();
    }
}

// Implementação da função de obter histórico
std::vector<std::string> SharedState::get_history() {
    std::lock_guard<std::mutex> lock(mtx);
    return std::vector<std::string>(message_history.begin(), message_history.end());
}