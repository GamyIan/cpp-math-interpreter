#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <sstream>
#include <map>
#include <optional>
#include "command.h"

using namespace std;

class Interpreter {
public:
    // If the statement is an expression, return its evaluated result.
    // If it's an assignment, perform the assignment and return nullopt.
    optional<double> execute(const string &input);
    
private:
    // Parser functions that build an expression tree of Command objects.
    CommandPtr parseExpression(stringstream &ss);
    CommandPtr parseFactor(stringstream &ss);
    CommandPtr parseExponent(stringstream &ss);
    CommandPtr parsePrimary(stringstream &ss);
    CommandPtr parseFunctionCall(const string &funcName, stringstream &ss);

    // Map for storing variables.
    map<string, double> variables;
};

#endif
