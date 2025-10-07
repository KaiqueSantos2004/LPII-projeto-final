#include "chat/Server.hpp"  
#include "chat/Client.hpp" 
#include "utils/Logger.hpp" 
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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " [server|client] [opções]" << std::endl;
        std::cerr << "  - server <porta>" << std::endl;
        std::cerr << "  - client <ip> <porta>" << std::endl;
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "server") {
        if (argc != 3) {
            std::cerr << "Uso: " << argv[0] << " server <porta>" << std::endl;
            return 1;
        }
        int port = std::stoi(argv[2]);
        Server server(port, "server_log.txt", "forbidden_words.txt");
        server.run();
    } 
    else if (mode == "client") {
        if (argc != 4) {
            std::cerr << "Uso: " << argv[0] << " client <ip> <porta>" << std::endl;
            return 1;
        }
        std::string ip = argv[2];
        int port = std::stoi(argv[3]);
        
        Client client(ip, port);
        if (client.connect_to_server()) {
            client.run();
        }
        else {
            // Esta lógica agora irá funcionar como esperado
            std::cerr << "\nFalha ao conectar ao servidor. Verifique o IP e a porta e tente novamente." << std::endl;
            std::cerr << "Pressione Enter para sair..." << std::endl;
            // Limpa qualquer estado de erro do cin e espera por um Enter
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
    }
    } 
    else {
        std::cerr << "Modo inválido. Use 'server' ou 'client'." << std::endl;
        return 1;
    }

    return 0;
}