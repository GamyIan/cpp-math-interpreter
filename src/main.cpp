#include "../include/interpreter.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "Advanced C++ Interpreter\nType 'exit' to quit.\n";
    Interpreter interpreter;
    std::string input;
    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, input);
        if (input == "exit" or input == "q") break;
        try {
            double result = interpreter.execute(input);
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "[ERROR]: " << e.what() << std::endl;
        }
    }
    return 0;
}
