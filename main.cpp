#include "Logger.hpp"
#include <vector>
#include <sstream>

void worker_function(Logger& logger, int thread_id) {
    for (int i = 0; i < 100; ++i) {
        std::stringstream ss;
        ss << "Mensagem " << i+1 << " da Thread " << thread_id;
        logger.log(ss.str());
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    std::cout << "Iniciando teste do Logger thread-safe..." << std::endl;

    Logger logger("test_log.txt");

    const int num_threads = 10;
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker_function, std::ref(logger), i + 1);
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "Teste concluído. Verifique o arquivo 'test_log.txt'." << std::endl;

    return 0;
}