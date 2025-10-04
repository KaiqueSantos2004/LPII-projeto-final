#include "utils/Logger.hpp"

Logger::Logger(const std::string& filename) : exit_flag(false) {
    try {
        std::filesystem::path current_path = std::filesystem::current_path();
        std::filesystem::path log_path = current_path / filename;
        std::cerr << "[DEBUG] Tentando abrir o log em: " << log_path.string() << std::endl;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "[DEBUG] Erro ao obter o caminho do arquivo: " << e.what() << std::endl;
    }

    log_file.open(filename, std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo de log.");
    }
    writer_thread = std::thread(&Logger::writer_thread_func, this);
}

Logger::~Logger() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        exit_flag = true;
    }
    condition.notify_one();
    if (writer_thread.joinable()) {
        writer_thread.join();
    }
    if (log_file.is_open()) {
        log_file.close();
    }
}

void Logger::log(const std::string& message) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        log_queue.push(message);
    }
    condition.notify_one();
}

void Logger::writer_thread_func() {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        condition.wait(lock, [this] { return !log_queue.empty() || exit_flag; });

        if (exit_flag && log_queue.empty()) {
            break;
        }

        if (!log_queue.empty()) {
            std::string message = log_queue.front();
            log_queue.pop();
            lock.unlock();

            if (log_file.is_open()) {
                log_file << message << std::endl;
            }
        }
    }
}