#include "../include/interpreter.h"
#include "../include/preprocessor.h"  // If you're using a preprocessor module.
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>

using namespace std;

int main(int argc, char* argv[]) {
    Interpreter interpreter;
    
    if (argc > 1) {
        // File mode.
        string filename = argv[1];
        ifstream infile(filename);
        if (!infile) {
            cerr << "Error: Unable to open file " << filename << endl;
            return 1;
        }
        string processedInput = preprocessInput(infile);
        istringstream iss(processedInput);
        string statement;
        while (getline(iss, statement)) {
            if (statement.find_first_not_of(" \t\n\r") == string::npos)
                continue;
            try {
                optional<double> result = interpreter.execute(statement);
                if (result.has_value())
                    cout << result.value() << endl;
            } catch (const exception &e) {
                cerr << "[ERROR]: " << e.what() << endl;
            }
        }
    } else {
        // Interactive mode.
        cout << "c-- Interpreter (Type 'exit' to quit)" << endl;
        while (true) {
            cout << ">> ";
            string line;
            getline(cin, line);
            if (line == "exit")
                break;
            try {
                optional<double> result = interpreter.execute(line);
                if (result.has_value())
                    cout << result.value() << endl;
            } catch (const exception &e) {
                cerr << "[ERROR]: " << e.what() << endl;
            }
        }
    }
    return 0;
}
