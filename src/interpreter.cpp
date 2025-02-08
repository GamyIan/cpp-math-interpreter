#include "../include/interpreter.h"
#include "../include/globals.h"
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

// The execute() function also supports assignment if an '=' is found.
double Interpreter::execute(const string &input) {
    size_t pos = input.find('=');
    if (pos != string::npos) {
        // Assignment: left side is variable name, right side is expression.
        string varName = input.substr(0, pos);
        // Remove whitespace from varName
        varName.erase(remove_if(varName.begin(), varName.end(), ::isspace), varName.end());
        string expr = input.substr(pos + 1);
        stringstream ss(expr);
        CommandPtr cmd = parseExpression(ss);
        double value = cmd->execute();
        variables[varName] = value;
        return value;
    } else {
        stringstream ss(input);
        CommandPtr cmd = parseExpression(ss);
        return cmd->execute();
    }
}

// parseExpression: handles addition and subtraction.
CommandPtr Interpreter::parseExpression(stringstream &ss) {
    CommandPtr left = parseFactor(ss);
    ss >> ws;
    while (ss.peek() == '+' || ss.peek() == '-') {
        char op = ss.get();
        CommandPtr right = parseFactor(ss);
        if (op == '+')
            left = make_shared<AddCommand>(left, right);
        else
            left = make_shared<SubCommand>(left, right);
        ss >> ws;
    }
    return left;
}

// parseFactor: handles multiplication and division.
CommandPtr Interpreter::parseFactor(stringstream &ss) {
    CommandPtr left = parseExponent(ss);
    ss >> ws;
    while (ss.peek() == '*' || ss.peek() == '/') {
        char op = ss.get();
        CommandPtr right = parseExponent(ss);
        if (op == '*')
            left = make_shared<MulCommand>(left, right);
        else
            left = make_shared<DivCommand>(left, right);
        ss >> ws;
    }
    return left;
}

// parseExponent: handles exponentiation (right-associative).
CommandPtr Interpreter::parseExponent(stringstream &ss) {
    CommandPtr left = parsePrimary(ss);
    ss >> ws;
    if (ss.peek() == '^') {
        ss.get(); // consume '^'
        CommandPtr right = parseExponent(ss); // right-associative
        left = make_shared<ExpCommand>(left, right);
    }
    return left;
}

// parsePrimary: handles numbers, parentheses, variable usage, and function calls.
CommandPtr Interpreter::parsePrimary(stringstream &ss) {
    ss >> ws;
    if (ss.peek() == '(') {
        ss.get(); // consume '('
        CommandPtr cmd = parseExpression(ss);
        ss >> ws;
        if (ss.peek() == ')')
            ss.get(); // consume ')'
        else
            throw runtime_error("Missing closing parenthesis");
        return cmd;
    }
    
    if (isalpha(ss.peek())) {
        string identifier;
        while (isalnum(ss.peek())) {
            identifier.push_back(ss.get());
        }
        ss >> ws;
        if (ss.peek() == '(') { // function call detected
            ss.get(); // consume '('
            return parseFunctionCall(identifier, ss);
        } else {
            // Variable lookup
            auto it = variables.find(identifier);
            if (it != variables.end())
                return make_shared<NumberCommand>(it->second);
            else
                throw runtime_error("Undefined variable: " + identifier);
        }
    }
    
    double num;
    ss >> num;
    return make_shared<NumberCommand>(num);
}

// parseFunctionCall: parses a function call with arguments.
CommandPtr Interpreter::parseFunctionCall(const string &funcName, stringstream &ss) {
    vector<CommandPtr> args;
    ss >> ws;
    if (ss.peek() != ')') {  // if there are arguments
        while (true) {
            CommandPtr arg = parseExpression(ss);
            args.push_back(arg);
            ss >> ws;
            if (ss.peek() == ',') {
                ss.get(); // consume comma
                ss >> ws;
            } else {
                break;
            }
        }
    }
    if (ss.peek() == ')')
        ss.get(); // consume ')'
    else
        throw runtime_error("Missing closing parenthesis in function call");
    
    // Create a FunctionCommand for the function call.
    return make_shared<FunctionCommand>(funcName, args);
}
