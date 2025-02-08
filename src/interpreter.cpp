#include "../include/interpreter.h"
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;  // Using namespace std for cleaner code

double Interpreter::execute(const string &input) {
    // Check if the input contains an assignment operator '='
    size_t pos = input.find('=');
    if (pos != string::npos) {
        // Assignment: parse variable name and expression
        string varName = input.substr(0, pos);
        // Remove whitespace from varName
        varName.erase(remove_if(varName.begin(), varName.end(), ::isspace), varName.end());
        string expr = input.substr(pos + 1);
        stringstream ss(expr);
        double value = evaluateExpression(ss);
        variables[varName] = value;
        return value;
    } else {
        stringstream ss(input);
        return evaluateExpression(ss);
    }
}

// Handles addition and subtraction
double Interpreter::evaluateExpression(stringstream &ss) {
    double left = evaluateFactor(ss);
    while (true) {
        ss >> ws; // Consume any whitespace
        char op = ss.peek();
        if (op == '+' || op == '-') {
            ss.get();
            double right = evaluateFactor(ss);
            left = (op == '+') ? (left + right) : (left - right);
        } else {
            break;
        }
    }
    return left;
}

// Handles multiplication and division
double Interpreter::evaluateFactor(stringstream &ss) {
    double left = evaluateExponent(ss);
    while (true) {
        ss >> ws;
        char op = ss.peek();
        if (op == '*' || op == '/') {
            ss.get();
            double right = evaluateExponent(ss);
            if (op == '*') {
                left *= right;
            } else {
                if (right == 0)
                    throw runtime_error("Division by zero");
                left /= right;
            }
        } else {
            break;
        }
    }
    return left;
}

// Handles exponentiation (right-associative)
double Interpreter::evaluateExponent(stringstream &ss) {
    double base = evaluatePrimary(ss);
    ss >> ws;
    if (ss.peek() == '^') {
        ss.get(); // Consume '^'
        double exponent = evaluateExponent(ss); // Recursive call for right-associativity
        base = pow(base, exponent);
    }
    return base;
}

// Handles numbers, parentheses, variable usage, and function calls
double Interpreter::evaluatePrimary(stringstream &ss) {
    ss >> ws;
    if (ss.peek() == '(') {
        ss.get(); // Consume '('
        double value = evaluateExpression(ss);
        ss >> ws;
        if (ss.peek() == ')') {
            ss.get(); // Consume ')'
        } else {
            throw runtime_error("Missing closing parenthesis");
        }
        return value;
    }
    
    if (isalpha(ss.peek())) {
        string identifier;
        while (isalnum(ss.peek())) {
            identifier.push_back(ss.get());
        }
        ss >> ws;
        if (ss.peek() == '(') { // Function call detected
            ss.get(); // Consume '('
            return evaluateFunctionCall(identifier, ss);
        } else {
            auto it = variables.find(identifier);
            if (it != variables.end()) {
                return it->second;
            } else {
                throw runtime_error("Undefined variable: " + identifier);
            }
        }
    }
    
    double number;
    ss >> number;
    return number;
}

// Parses a function call and evaluates it.
// Expects that the '(' has already been consumed.
double Interpreter::evaluateFunctionCall(const string &funcName, stringstream &ss) {
    vector<double> args;
    ss >> ws;
    if (ss.peek() != ')') {  // If there are arguments
        while (true) {
            double arg = evaluateExpression(ss);
            args.push_back(arg);
            ss >> ws;
            if (ss.peek() == ',') {
                ss.get(); // Consume comma
                ss >> ws;
            } else {
                break;
            }
        }
    }
    if (ss.peek() == ')') {
        ss.get(); // Consume closing parenthesis
    } else {
        throw runtime_error("Missing closing parenthesis in function call");
    }
    
    // Allow variable number of arguments for add, sub, mul, and div.
    if (funcName == "add") {
        if (args.empty()) throw runtime_error("add() expects at least one argument");
        double sum = 0;
        for (double a : args) sum += a;
        return sum;
    } else if (funcName == "sub") {
        if (args.empty()) throw runtime_error("sub() expects at least one argument");
        if (args.size() == 1) return args[0];  // or return -args[0] if you prefer
        double result = args[0];
        for (size_t i = 1; i < args.size(); ++i) {
            result -= args[i];
        }
        return result;
    } else if (funcName == "mul") {
        if (args.empty()) throw runtime_error("mul() expects at least one argument");
        double product = 1;
        for (double a : args) product *= a;
        return product;
    } else if (funcName == "div") {
        if (args.empty()) throw runtime_error("div() expects at least one argument");
        if (args.size() == 1) return args[0];
        double result = args[0];
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == 0) throw runtime_error("Division by zero");
            result /= args[i];
        }
        return result;
    } else if (funcName == "pow") {
        if (args.size() != 2)
            throw runtime_error("pow() expects exactly 2 arguments");
        return pow(args[0], args[1]);
    } else if (funcName == "sqrt") {
        if (args.size() != 1)
            throw runtime_error("sqrt() expects exactly 1 argument");
        return sqrt(args[0]);
    } else if (funcName == "sin") {
        if (args.size() != 1)
            throw runtime_error("sin() expects exactly 1 argument");
        return sin(args[0]);
    } else if (funcName == "cos") {
        if (args.size() != 1)
            throw runtime_error("cos() expects exactly 1 argument");
        return cos(args[0]);
    } else if (funcName == "tan") {
        if (args.size() != 1)
            throw runtime_error("tan() expects exactly 1 argument");
        return tan(args[0]);
    } else if (funcName == "asin") {
        if (args.size() != 1)
            throw runtime_error("asin() expects exactly 1 argument");
        return asin(args[0]);
    } else if (funcName == "acos") {
        if (args.size() != 1)
            throw runtime_error("acos() expects exactly 1 argument");
        return acos(args[0]);
    } else if (funcName == "atan") {
        if (args.size() != 1)
            throw runtime_error("atan() expects exactly 1 argument");
        return atan(args[0]);
    } else {
        throw runtime_error("Unknown function: " + funcName);
    }
}


