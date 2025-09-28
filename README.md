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
