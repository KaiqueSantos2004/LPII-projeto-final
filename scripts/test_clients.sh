#!/bin/bash

# --- Configurações ---
SERVER_IP="127.0.0.1"
PORT=8080
NUM_CLIENTS=5

# Caminhos absolutos para robustez
PROJECT_DIR=$(pwd)
EXECUTABLE="$PROJECT_DIR/bin/chat_app"
# ---------------------

if [ ! -f "$EXECUTABLE" ]; then
    echo "Erro: Executável '$EXECUTABLE' não encontrado."
    exit 1
fi

echo "Iniciando $NUM_CLIENTS clientes de teste em segundo plano..."

for (( i=1; i<=$NUM_CLIENTS; i++ ))
do
    # Envia uma única mensagem e fecha a conexão de entrada depois de 1 segundo.
    # O '&' executa o cliente em segundo plano.
    (echo "Olá, sou o cliente de teste $i"; sleep 1) | "$EXECUTABLE" client "$SERVER_IP" "$PORT" &
    
    # Pequeno atraso para não sobrecarregar o servidor com conexões simultâneas
    sleep 0.1
done

echo "$NUM_CLIENTS clientes de teste iniciados."
echo "Eles devem se desconectar sozinhos após enviarem a mensagem."
echo "Para garantir que todos foram encerrados, use o comando: pkill -f '$EXECUTABLE client'"