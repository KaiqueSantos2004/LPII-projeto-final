#ifndef SHARED_STATE_HPP
#define SHARED_STATE_HPP

#include <vector>
#include <mutex>
#include <deque>
#include <string>

class SharedState {
public:
    SharedState();
    void add_client(int client_socket);
    void remove_client(int client_socket);
    std::vector<int> get_all_clients();

    void add_message(const std::string& message);
    std::vector<std::string> get_history();

private:
    std::vector<int> clients;
    std::mutex mtx;
    std::deque<std::string> message_history;
    const size_t MAX_HISTORY_SIZE = 10;
};

#endif // SHARED_STATE_HPP