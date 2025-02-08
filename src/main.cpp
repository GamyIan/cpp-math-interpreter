#include "../include/interpreter.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    cout << "Command Pattern C++ Interpreter\nType 'exit' to quit.\n";
    Interpreter interpreter;
    string input;
    while (true) {
        cout << ">> ";
        getline(cin, input);
        if (input == "exit" or input == "q")
            break;
        try {
            double result = interpreter.execute(input);
            cout << "Result: " << result << endl;
        } catch (const exception &e) {
            cerr << "[ERROR]: " << e.what() << endl;
        }
    }
    return 0;
}
