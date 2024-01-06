
#ifndef SHELL_H
#define SHELL_H

#include <vector>
#include <string>
#include <map>

// user includes
#include "command_parser.hpp"
#include "file_system.hpp"
#include "memory_manager.hpp"
#include "commands.hpp"
#include "directory.hpp"

class Shell {
private:
    const std::string mount = "./file_system";
    std::map<std::string, Command*> commands;
    MemoryManager* memory_manager;
    Directory current_dir;
    void boot();

public:
    Shell();

    void add_command(const std::string& name, Command* cmd);
    void execute_command(Command& cmd);
    void execute_command(const std::string& input);
    void flush() const;

    File* find_file(const std::string& path);


    friend class Command;
};

#endif