#include "../include/interpreter.h"
#include "../include/globals.h"
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <optional>

using namespace std;

// The execute() method now returns an optional<double> so that assignments produce no output.
optional<double> Interpreter::execute(const string &input) {
    // Check if there's an assignment operator '=' in the statement.
    size_t pos = input.find('=');
    if (pos != string::npos) {
        // Assignment: left-hand side is the variable name.
        string varName = input.substr(0, pos);
        // Remove whitespace from varName.
        varName.erase(remove_if(varName.begin(), varName.end(), ::isspace), varName.end());
        string expr = input.substr(pos + 1);
        stringstream ss(expr);
        CommandPtr cmd = parseExpression(ss);
        double value = cmd->execute();
        variables[varName] = value;
        return nullopt;  // For assignments, return no output.
    } else {
        stringstream ss(input);
        CommandPtr cmd = parseExpression(ss);
        double result = cmd->execute();
        return result;
    }
}

// parseExpression: Handles addition and subtraction.
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

// parseFactor: Handles multiplication and division.
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

// parseExponent: Handles exponentiation (right-associative).
CommandPtr Interpreter::parseExponent(stringstream &ss) {
    CommandPtr left = parsePrimary(ss);
    ss >> ws;
    if (ss.peek() == '^') {
        ss.get(); // Consume '^'
        CommandPtr right = parseExponent(ss); // Right-associative
        left = make_shared<ExpCommand>(left, right);
    }
    return left;
}

// parsePrimary: Handles numbers, parentheses, variables, and function calls.
CommandPtr Interpreter::parsePrimary(stringstream &ss) {
    ss >> ws;
    if (ss.peek() == '(') {
        ss.get(); // Consume '('
        CommandPtr cmd = parseExpression(ss);
        ss >> ws;
        if (ss.peek() == ')')
            ss.get(); // Consume ')'
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
        if (ss.peek() == '(') { // Function call detected.
            ss.get(); // Consume '('.
            return parseFunctionCall(identifier, ss);
        } else {
            // Variable lookup.
            auto it = variables.find(identifier);
            if (it != variables.end())
                return make_shared<NumberCommand>(it->second);
            else
                throw runtime_error("Undefined variable: " + identifier);
        }
    }
    
    // Otherwise, read a number.
    double num;
    ss >> num;
    return make_shared<NumberCommand>(num);
}

// parseFunctionCall: Parses a function call and its arguments.
CommandPtr Interpreter::parseFunctionCall(const string &funcName, stringstream &ss) {
    vector<CommandPtr> args;
    ss >> ws;
    if (ss.peek() != ')') {  // If there are arguments.
        while (true) {
            CommandPtr arg = parseExpression(ss);
            args.push_back(arg);
            ss >> ws;
            if (ss.peek() == ',') {
                ss.get(); // Consume comma.
                ss >> ws;
            } else {
                break;
            }
        }
    }
    if (ss.peek() == ')')
        ss.get(); // Consume closing parenthesis.
    else
        throw runtime_error("Missing closing parenthesis in function call");
    
    return make_shared<FunctionCommand>(funcName, args);
}
