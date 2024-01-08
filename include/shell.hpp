
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

const std::string mount_path = "./file_system";
class Shell {
private:

    std::map<std::string, Command*> commands;
    MemoryManager* memory_manager;

    Directory* current_dir;
    Directory root_dir;

    // Functions
    void boot();
    void load_from_directory(Directory* dir);
    void _flush(const Directory* dir);

public:
    Shell();
    void interactive();

    void add_command(const std::string& name, Command* cmd);
    void execute_command(Command& cmd);
    void execute_command(const std::string& input);
    void flush();

    Directory* get_current_dir() {return current_dir;}
    void set_current_dir(Directory* dir) {current_dir = dir;}

    friend class Command;
};

#endif