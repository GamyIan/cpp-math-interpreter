#include "../include/command.h"
#include "../include/globals.h"
#include <cmath>
#include <stdexcept>

using namespace std;

// --- NumberCommand ---
NumberCommand::NumberCommand(double val) : value(val) {}

double NumberCommand::execute() const {
    return value;
}

// --- BinaryCommand ---
BinaryCommand::BinaryCommand(CommandPtr l, CommandPtr r)
    : left(l), right(r) {}

// --- AddCommand ---
AddCommand::AddCommand(CommandPtr l, CommandPtr r)
    : BinaryCommand(l, r) {}

double AddCommand::execute() const {
    return left->execute() + right->execute();
}

// --- SubCommand ---
SubCommand::SubCommand(CommandPtr l, CommandPtr r)
    : BinaryCommand(l, r) {}

double SubCommand::execute() const {
    return left->execute() - right->execute();
}

// --- MulCommand ---
MulCommand::MulCommand(CommandPtr l, CommandPtr r)
    : BinaryCommand(l, r) {}

double MulCommand::execute() const {
    return left->execute() * right->execute();
}

// --- DivCommand ---
DivCommand::DivCommand(CommandPtr l, CommandPtr r)
    : BinaryCommand(l, r) {}

double DivCommand::execute() const {
    double denom = right->execute();
    if (denom == 0)
        throw runtime_error("Division by zero");
    return left->execute() / denom;
}

// --- ExpCommand ---
ExpCommand::ExpCommand(CommandPtr l, CommandPtr r)
    : BinaryCommand(l, r) {}

double ExpCommand::execute() const {
    return pow(left->execute(), right->execute());
}

// --- FunctionCommand ---
// Constructor
FunctionCommand::FunctionCommand(const string &name, const vector<CommandPtr> &args)
    : funcName(name), arguments(args)
{
}

double FunctionCommand::execute() const {
    if (funcName == "add") {
        if (arguments.empty())
            throw runtime_error("add() expects at least one argument");
        double sum = 0;
        for (auto &arg : arguments)
            sum += arg->execute();
        return sum;
    } else if (funcName == "sub") {
        if (arguments.empty())
            throw runtime_error("sub() expects at least one argument");
        double result = arguments[0]->execute();
        for (size_t i = 1; i < arguments.size(); i++)
            result -= arguments[i]->execute();
        return result;
    } else if (funcName == "mul") {
        if (arguments.empty())
            throw runtime_error("mul() expects at least one argument");
        double product = 1;
        for (auto &arg : arguments)
            product *= arg->execute();
        return product;
    } else if (funcName == "div") {
        if (arguments.empty())
            throw runtime_error("div() expects at least one argument");
        double result = arguments[0]->execute();
        for (size_t i = 1; i < arguments.size(); i++) {
            double denom = arguments[i]->execute();
            if (denom == 0)
                throw runtime_error("Division by zero");
            result /= denom;
        }
        return result;
    } else if (funcName == "pow") {
        if (arguments.size() != 2)
            throw runtime_error("pow() expects 2 arguments");
        return pow(arguments[0]->execute(), arguments[1]->execute());
    } else if (funcName == "sqrt") {
        if (arguments.size() != 1)
            throw runtime_error("sqrt() expects 1 argument");
        return sqrt(arguments[0]->execute());
    }
    // Trigonometric functions with optional override.
    else if (funcName == "sin") {
        if (arguments.size() < 1 || arguments.size() > 2)
            throw runtime_error("sin() expects 1 or 2 arguments");
        double val = arguments[0]->execute();
        bool useDegrees;
        if (arguments.size() == 2)
            useDegrees = (arguments[1]->execute() == 0);  // 0 means degrees, 1 means radians
        else
            useDegrees = (currentAngleMode == AngleMode::Degrees);
        if (useDegrees)
            val = val * M_PI / 180.0;
        return sin(val);
    } else if (funcName == "cos") {
        if (arguments.size() < 1 || arguments.size() > 2)
            throw runtime_error("cos() expects 1 or 2 arguments");
        double val = arguments[0]->execute();
        bool useDegrees;
        if (arguments.size() == 2)
            useDegrees = (arguments[1]->execute() == 0);
        else
            useDegrees = (currentAngleMode == AngleMode::Degrees);
        if (useDegrees)
            val = val * M_PI / 180.0;
        return cos(val);
    } else if (funcName == "tan") {
        if (arguments.size() < 1 || arguments.size() > 2)
            throw runtime_error("tan() expects 1 or 2 arguments");
        double val = arguments[0]->execute();
        bool useDegrees;
        if (arguments.size() == 2)
            useDegrees = (arguments[1]->execute() == 0);
        else
            useDegrees = (currentAngleMode == AngleMode::Degrees);
        if (useDegrees)
            val = val * M_PI / 180.0;
        return tan(val);
    } else if (funcName == "asin") {
        if (arguments.size() < 1 || arguments.size() > 2)
            throw runtime_error("asin() expects 1 or 2 arguments");
        double result = asin(arguments[0]->execute());
        bool outputDegrees;
        if (arguments.size() == 2)
            outputDegrees = (arguments[1]->execute() == 0);
        else
            outputDegrees = (currentAngleMode == AngleMode::Degrees);
        if (outputDegrees)
            result = result * 180.0 / M_PI;
        return result;
    } else if (funcName == "acos") {
        if (arguments.size() < 1 || arguments.size() > 2)
            throw runtime_error("acos() expects 1 or 2 arguments");
        double result = acos(arguments[0]->execute());
        bool outputDegrees;
        if (arguments.size() == 2)
            outputDegrees = (arguments[1]->execute() == 0);
        else
            outputDegrees = (currentAngleMode == AngleMode::Degrees);
        if (outputDegrees)
            result = result * 180.0 / M_PI;
        return result;
    } else if (funcName == "atan") {
        if (arguments.size() < 1 || arguments.size() > 2)
            throw runtime_error("atan() expects 1 or 2 arguments");
        double result = atan(arguments[0]->execute());
        bool outputDegrees;
        if (arguments.size() == 2)
            outputDegrees = (arguments[1]->execute() == 0);
        else
            outputDegrees = (currentAngleMode == AngleMode::Degrees);
        if (outputDegrees)
            result = result * 180.0 / M_PI;
        return result;
    } else {
        throw runtime_error("Unknown function: " + funcName);
    }
}
