#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <fstream>
#include <string>

// user includes
#include "memory_manager.hpp"
#include "file_system.hpp"
//#include "shell.hpp"

class Shell; // forward declaration

// Command Design Pattern

// Command interface as ABC
class Command {
private:
    virtual void _execute() = 0;

protected:
    Directory* current_dir;

public:
    virtual void execute(Shell& shell);
    virtual void execute(Shell& shell, const std::vector<std::string>& params) = 0;
};

class ExitCommand : public Command {
private:
    void _execute() override;
    bool flush;

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

    void _list_directory(const Directory* dir) const;

public:
    LsCommand() : recursive(false) {}
    LsCommand(bool recursive) : recursive(recursive) {}

    void execute(Shell& shell, const bool recursive=false);
    void execute(Shell& shell, const std::vector<std::string>& params) override;
};


class CatCommand : public Command {
private:
    std::string fname;
    void _execute() override;

public:
    CatCommand(){};
    void execute(Shell& shell, const std::vector<std::string>& params) override;
};


class CdCommand : public Command {
private:
    std::string dname;
    Directory* found_dir;
    void _execute() override;
public:
    void execute(Shell& shell, const std::vector<std::string>& params) override;
};

class RmCommand : public Command {
private:
    std::string fname;
    void _execute() override;
public:
    void execute(Shell& shell, const std::vector<std::string>& params) override;
};

class LinkCommand : public Command {
private:
    std::string _src;
    std::string _dest;
    void _execute() override;
public:
    void execute(Shell& shell, const std::vector<std::string>& params) override;
};

class MkdirCommand : public Command {
private:
    std::string dname;
    void _execute() override;
public:
    void execute(Shell& shell, const std::vector<std::string>& params) override;
};

class MMCommand : public Command {
private:
    void _execute() override;
public:
    void execute(Shell& shell, const std::vector<std::string>& params) override {_execute();};
};

class FlushCommand : public Command {
private:
    void _execute() override {};
public:
    void execute(Shell& shell, const std::vector<std::string>& params) override;
};

#endif