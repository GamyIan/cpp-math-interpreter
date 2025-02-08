#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <sstream>
#include <map>

using namespace std;  // Using namespace std for cleaner code

class Interpreter {
public:
    double execute(const string &input);
    
private:
    // Expression parsing functions with correct precedence
    double evaluateExpression(stringstream &ss);
    double evaluateFactor(stringstream &ss);
    double evaluateExponent(stringstream &ss);
    double evaluatePrimary(stringstream &ss);

    // Helper: parses and evaluates function calls like add(), sub(), etc.
    double evaluateFunctionCall(const string &funcName, stringstream &ss);

    // Map for storing variables
    map<string, double> variables;
};

#endif
