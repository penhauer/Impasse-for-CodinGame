export CXX=g++
export CXXFLAGS=-O3
# export CXXFLAGS=-g


SRC_DIR := ./
OBJ_DIR := ./build
BIN_DIR := ./

EXCLUDED_FILES := ai.cpp combined.cpp


SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
SRC_FILES := $(filter-out $(addprefix $(SRC_DIR)/,$(EXCLUDED_FILES)), $(SRC_FILES))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
TARGET_NAME = main
COMBINED_TARGET=combined



combine:
	cat common.h board.h state.h player.h game.h minimax_player.h random_player.h simple_terminal_player.h mcts_player.h common.cpp board.cpp state.cpp game.cpp minimax_player.cpp random_player.cpp simple_terminal_player.cpp mcts_player.cpp main.cpp | grep  -E "pragma|#ifndef|#endif|#define|state.h|common.h|random_player.h|player.h|state.h|board.h|game.h" -v > combined.cpp; \
		$(CXX) $(CXXFLAGS) $(COMBINED_TARGET).cpp -o $(COMBINED_TARGET)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


all: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $(OBJ_DIR)/$(TARGET_NAME)





run: all
	$(OBJ_DIR)/$(TARGET_NAME)


clean:
	rm -rf main build/* 

.PHONY: clean combine
