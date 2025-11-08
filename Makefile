CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude -g

SRC_DIR := src
BIN_DIR := bin

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRC))

TARGET := $(BIN_DIR)/meow

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ) -o $@
	@echo "Build complete: $@"

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)
	@echo "Cleaned build directory."

run: all
	@./$(TARGET)

