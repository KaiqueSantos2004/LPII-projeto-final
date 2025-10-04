#ifndef SHARED_STATE_HPP
#define SHARED_STATE_HPP

#include <vector>
#include <mutex>

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