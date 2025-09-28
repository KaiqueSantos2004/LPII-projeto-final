#include "Logger.hpp"

//contrutor abre o arquivo de log e inicia a thread de escrita
Logger::Logger(const std::string& filename) : exit_flag(false) {
    log_file.open(filename, std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo de log.");
    }
    //inicia a thread de escrita em segundo plano
    writer_thread = std::thread(&Logger::writer_thread_func, this);
}
//Destrutor: 
Logger::~Logger() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        exit_flag = true;
    }
    //Notifica a thread de escrita que pode estar esperando na variável de condição para sair
    cv.notify_one();
    if (writer_thread.joinable()) {
        writer_thread.join();
    }
    if (log_file.is_open()) {
        log_file.close();
    }
}


void Logger::log(const std::string& message) {
    {
        //Adquire o lock, e então ele adiciona a mensagem a fila, garantindo que o acesso à fila é thread-safe
        std::unique_lock<std::mutex> lock(mtx);
        message_queue.push(message);
    }
    //Notifica a thread de escrita sobre a nova mensagem adicionada
    cv.notify_one();
}

//Função executada pela thread consumidora
void Logger::writer_thread_func() {
    while (true) {
        
        std::unique_lock<std::mutex> lock(mtx);

        //Quando notificada, a thread de escrita acorda, tenta adquirir o lock e verifica se há mensagens na fila ou se deve sair
        cv.wait(lock, [this] { return !message_queue.empty() || exit_flag; });

        //Se a exit_flag estiver setada e fila vazia, então termina a execuação
        if (exit_flag && message_queue.empty()) {
            break;
        }

        //Caso contrário, processa todas as mensagens na fila, deixando o lock  liberado para que outras threads adicionem novas mensagens
        if (!message_queue.empty()) {
            std::string message = message_queue.front();
            message_queue.pop();
            lock.unlock(); 
            log_file << message << std::endl;
        }
    }
}