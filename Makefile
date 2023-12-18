export CXX=g++
export CXXFLAGS=-O3


SRC_DIR := ./
OBJ_DIR := ./build
BIN_DIR := ./

EXCLUDED_FILES := ai.cpp


SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
SRC_FILES := $(filter-out $(addprefix $(SRC_DIR)/,$(EXCLUDED_FILES)), $(SRC_FILES))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
TARGET_NAME = main


clean:
	rm -rf main build/*

.PHONY: clean

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


all: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $(OBJ_DIR)/$(TARGET_NAME)


run: all
	$(OBJ_DIR)/$(TARGET_NAME)


