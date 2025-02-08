#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cmath>

using namespace std;

// Base Command interface
class Command {
public:
    virtual ~Command() {}
    virtual double execute() const = 0;
};

typedef shared_ptr<Command> CommandPtr;

// Command that returns a number (a leaf in the expression tree)
class NumberCommand : public Command {
    double value;
public:
    NumberCommand(double val);
    double execute() const override;
};

// Base class for binary operations
class BinaryCommand : public Command {
protected:
    CommandPtr left;
    CommandPtr right;
public:
    BinaryCommand(CommandPtr l, CommandPtr r);
};

// Concrete commands for binary operations
class AddCommand : public BinaryCommand {
public:
    AddCommand(CommandPtr l, CommandPtr r);
    double execute() const override;
};

class SubCommand : public BinaryCommand {
public:
    SubCommand(CommandPtr l, CommandPtr r);
    double execute() const override;
};

class MulCommand : public BinaryCommand {
public:
    MulCommand(CommandPtr l, CommandPtr r);
    double execute() const override;
};

class DivCommand : public BinaryCommand {
public:
    DivCommand(CommandPtr l, CommandPtr r);
    double execute() const override;
};

class ExpCommand : public BinaryCommand {
public:
    ExpCommand(CommandPtr l, CommandPtr r);
    double execute() const override;
};

// Command for functions (supports variable number of arguments)
class FunctionCommand : public Command {
    string funcName;
    vector<CommandPtr> arguments;
public:
    FunctionCommand(const string &name, const vector<CommandPtr> &args);
    double execute() const override;
};

#endif
