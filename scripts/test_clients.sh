#!/bin/bash

# --- Configurações ---
SERVER_IP="127.0.0.1"
PORT=8080
NUM_CLIENTS=5
EXECUTABLE="./bin/chat_app"
# ---------------------

if [ ! -f "$EXECUTABLE" ]; then
    echo "Erro: Executável '$EXECUTABLE' não encontrado."
    echo "Por favor, compile o projeto primeiro com o comando:"
    echo "g++ main.cpp Server.cpp Client.cpp ClientHandler.cpp SharedState.cpp Logger.cpp -o chat_app -pthread -std=c++17"
    exit 1
fi

echo "Iniciando $NUM_CLIENTS clientes para conectar em $SERVER_IP:$PORT..."

#loop de início de cada cliente
for (( i=1; i<=$NUM_CLIENTS; i++ ))
do
    #Inicia cada cliente em um terminal separado
    #usa sleep para que possa ler a mensagem antes de fechar
    (echo "Olá, sou o cliente $i"; sleep 5) | $EXECUTABLE client $SERVER_IP $PORT &
done

echo "$NUM_CLIENTS clientes foram iniciados em novos terminais."
echo "Observe o terminal do servidor para ver as conexões chegando."
echo "Pressione Ctrl+C neste terminal para parar de monitorar."
echo "Para encerrar todos os clientes de uma vez, use o comando: pkill -f '$EXECUTABLE client'"

wait
