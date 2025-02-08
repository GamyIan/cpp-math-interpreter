CXX = g++
CXXFLAGS = -Wall -std=c++17
SRC = $(wildcard src/*.cpp)
INC = -Iinclude
TARGET = interpreter

all:
	$(CXX) $(CXXFLAGS) $(SRC) $(INC) -o $(TARGET)

clean:
	rm -f $(TARGET)
