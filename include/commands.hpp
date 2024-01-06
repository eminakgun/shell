#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <fstream>
#include <string>

// user includes
#include "memory_manager.hpp"

class Shell; // forward declaration

// Command Design Pattern

// Command interface as ABC
class Command {
private:
    virtual void _execute() = 0;

public:
    virtual void execute(Shell& shell);
    virtual void execute(Shell& shell, const std::vector<std::string>& params) = 0;
};

class ExitCommand : public Command {
private:
    void _execute() override;

public:
    void execute(Shell& shell, const std::vector<std::string>& params) override;
};

class CpCommand : public Command {
private:
    std::string _src;
    std::string _dest;
    void _execute() override;

public:
    CpCommand();
    CpCommand(std::string src, std::string dest);
    void execute(Shell& shell, const std::vector<std::string>& params) override;
};


class LsCommand : public Command {
private:
    void _execute() override;
    bool recursive;

public:
    LsCommand();
    void execute(Shell& shell, const std::vector<std::string>& params) override;
};

class CatCommand : public Command {
private:
    File* file;
    void _execute() override;

public:
    CatCommand();
    void execute(Shell& shell, const std::vector<std::string>& params) override;
};

#endif