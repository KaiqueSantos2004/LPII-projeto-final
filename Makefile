CXX = g++
# Flags de compilação: -std=c++17 para usar recursos modernos do C++, -g para debug, -Wall para avisos
CXXFLAGS = -std=c++17 -g -Wall
# Flags de linkagem: -pthread para a biblioteca de threads e -lstdc++fs para filesystem
LDFLAGS = -pthread -lstdc++fs

# Diretórios
INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Nome do executável final
TARGET = $(BIN_DIR)/chat_app

# Encontra todos os arquivos .cpp recursivamente no diretório src
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
# Gera os nomes dos arquivos objeto (.o) correspondentes na pasta de build
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# --- REGRAS ---

# Regra principal: é a primeira a ser executada (quando você digita 'make')
all: $(TARGET)

# Regra para linkar os arquivos objeto e criar o executável final
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Executável '$(TARGET)' criado com sucesso!"

# Regra para compilar cada arquivo .cpp em um arquivo .o
# -I$(INCLUDE_DIR) diz ao compilador para procurar os arquivos .hpp na pasta 'include'
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@
	@echo "Compilado $< -> $@"

# Regra para limpar os arquivos gerados
clean:
	@echo "Limpando arquivos gerados..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets são regras que não representam arquivos
.PHONY: all clean