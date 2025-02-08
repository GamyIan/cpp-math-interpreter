#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <sstream>
#include <map>
#include "command.h"

using namespace std;

class Interpreter {
public:
    double execute(const string &input);
    
private:
    // Parser functions that build an expression tree of Command objects.
    CommandPtr parseExpression(stringstream &ss);
    CommandPtr parseFactor(stringstream &ss);
    CommandPtr parseExponent(stringstream &ss);
    CommandPtr parsePrimary(stringstream &ss);
    CommandPtr parseFunctionCall(const string &funcName, stringstream &ss);

    // Map for storing variables (for variable lookup and assignment)
    map<string, double> variables;
};

#endif
