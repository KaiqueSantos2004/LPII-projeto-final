// Logger.hpp
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

class Logger {
public:
    //Construtor, abre o arquivo onde os logs serão salvos
    Logger(const std::string& filename);

    //Destrutor, para quando a thread de escrita for finalizada
    ~Logger();

    //Função para adicionar uma mensagem de log à fila de processamento
    void log(const std::string& message);

private:

    void writer_thread_func();

    std::ofstream           log_file;      
    std::queue<std::string> message_queue; //Fila onde as threads produtoras depositam as mensagens 
    
    std::mutex              mtx; //mutex que protege o acesso à fila de mensagens    
    std::condition_variable cv;  //variável de condição para notificar a thread de escrita sobre novas mensagens ou término
    bool                    exit_flag; //Flag que indica à writer_thread que deve terminar sua execução
    
    std::thread  writer_thread; //thread que faz a writer_thread_func
};



#endif // LOGGER_HPP