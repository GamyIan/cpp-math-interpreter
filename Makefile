CXX = g++
CXXFLAGS = -Wall -std=c++17
SRC = src/main.cpp src/interpreter.cpp
INC = -I../include
TARGET = interpreter

all:
	$(CXX) $(CXXFLAGS) $(SRC) $(INC) -o $(TARGET)

clean:
	rm -f $(TARGET)