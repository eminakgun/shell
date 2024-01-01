#include "commands.hpp"
#include "shell.hpp"

// Command Implementations
void Command::execute(Shell& shell) {
    _execute();
}

// Exit Command
void ExitCommand::_execute() {
    std::cout << "Exiting the shell." << std::endl;
    exit(0);
}

void ExitCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    shell.flush();
    _execute();
}

// Cp Command
CpCommand::CpCommand(){}
CpCommand::CpCommand(std::string src, std::string dest) : _src(src), _dest(dest) {}

void CpCommand::_execute() {
    // TODO check if source is outside of our file system
    // check if it exists
    std::ifstream src_file(_src);
    if (!src_file.is_open())
    {
        std::cout << "Error opening input file: " << _src << std::endl;
        return;
    }
    // read _src file into a string
    std::string content((std::istreambuf_iterator<char>(src_file)),
                             std::istreambuf_iterator<char>());
    src_file.close(); // Close the file
    
    // request a file from memory_manager and load it
    MemoryManager::get()->allocate_file(_dest, content);
}

void CpCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    // Copy
    _src = params[0];
    _dest = params[1];
    _execute();
}