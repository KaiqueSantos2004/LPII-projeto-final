# LPII-projeto-final

Entrega 1 - Biblioteca libtslog + Arquitetura

O que o programa faz:
Implementa a classe Logger com padrão consumidor-produtor onde múltiplas threads produtoras irão adicionar
mensagens a ser lidas de forma segura e são enfileiradas para serem consumidas por uma thread escritora 
que vai escrever tudo em um arquivo test_log.txt. Arquitetura de outras partes do sistema(Server.hpp, client.hpp, etc) são
classes vazias e  estão apenas como diagramas para o planejamento da próxima etapa

Comandos de compilação e execução:
 ```bash
g++ main.cpp Logger.cpp -o logger_test -pthread -std=c++17
./logger_test
```


Entrega 2 - Protótipo CLI de Comunicação 


Essa etapa do  projeto faz a implementação de um sistema de chat cliente/servidor TCP concorrente. O servidor é capaz de gerenciar múltiplas conexões de clientes simultaneamente, retransmitindo as mensagens por broadcast para outros participantes.

## Estrutura do Projeto

```
LPII-projeto-final/
|
|-- bin/              # Contém os executáveis compilados e arquivos de log
|-- build/            # Contém os arquivos objeto (.o) temporários da compilação
|-- include/          # Contém os arquivos de cabeçalho (.hpp)
|   |-- chat/
|   `-- utils/
|-- src/              # Contém os arquivos de implementação (.cpp)
|   |-- chat/
|   |-- utils/
|   `-- main.cpp
|-- scripts/          # Contém scripts para automação de testes
|-- Makefile          # Arquivo de build para compilação
`-- README.md
```

## Funcionalidades (Etapa 2)

* **Servidor Concorrente:** Aceita múltiplas conexões de clientes, criando uma thread dedicada para cada um.
* **Broadcast de Mensagens:** Mensagens enviadas por um cliente são retransmitidas para todos os outros clientes conectados.
* **Logging Thread-Safe:** Todas as atividades importantes (conexões, desconexões, mensagens) são registradas em um arquivo de log de forma segura, utilizando uma biblioteca `Logger` com padrão produtor-consumidor.
* **CLI Unificado:** Um único executável (`chat_app`) que pode ser iniciado em modo `server` ou `client` via argumentos de linha de comando.

## Compilação e Execução

O projeto utiliza um `Makefile` para automatizar o processo de compilação.

### Pré-requisitos
* Compilador C++ (g++)
* `make`
* Biblioteca `pthread` (padrão em sistemas Linux)

### Comandos de Compilação e Execução

1.  **Compilar o projeto:**
    A partir da pasta raiz do projeto, execute o comando:
    ```bash
    make
    ```
    Isso compilará todos os fontes e criará o executável em `bin/chat_app`.

2.  **Iniciar o Servidor:**
    Em um terminal, execute:
    ```bash
    ./bin/chat_app server <porta>
    ```
    Exemplo: `./bin/chat_app server 8080`

3.  **Iniciar um Cliente:**
    Em outro terminal, execute:
    ```bash
    ./bin/chat_app client <ip_do_servidor> <porta>
    ```
    Exemplo: `./bin/chat_app client 127.0.0.1 8080`

4.  **Limpar arquivos compilados:**
    Para remover as pastas `bin/` e `build/`, execute:
    ```bash
    make clean
    ```

### Simulação com  Múltiplos Clientes

Como rodar o script de teste  para simular a conexão de N clientes automaticamente.

1.  Com o servidor já rodando, abra um novo terminal.
2.  Dê permissão de execução ao script (apenas na primeira vez):
    ```bash
    chmod +x scripts/test_clients.sh
    ```
3.  Execute o script:
    ```bash
    ./scripts/test_clients.sh
    ```
    Isso iniciará 5 clientes em segundo plano que enviarão uma mensagem de teste. Se o usuário deseja 
    criar mais é necessário mudar o número de clientes no arquivo de script

# OBS:
Ao Fechar o servidor por exemplo com Ctrl+C, sistema operacional não libera a porta imediatamente, então é necessário
ou mudar a porta manualmente ou esperar um período de tempo (geralmente de 30 a 120 segundos).

