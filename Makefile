
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lssl -lcrypto

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = bin/securestore

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p bin
	$(CXX) $(OBJ) -o $(BIN) $(LDFLAGS)

clean:
	rm -rf $(OBJ) $(BIN)