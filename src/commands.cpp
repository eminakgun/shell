#include "commands.hpp"
#include "shell.hpp"

// Command Implementations
void Command::execute(Shell& shell) {
    current_dir = shell.get_current_dir();
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

    std::cout << _src << std::endl;
    std::string content = FileSystemHandler::read_file(_src);

    std::cout << "CpCommand current dir: " << current_dir->get_name() << std::endl;
    std::cout << "file content: " << content << std::endl;
    
    // request a file from memory_manager and load it
    File* file = MemoryManager::get()->allocate_file(_dest, content);
    current_dir->add_file(file);
}

void CpCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    // Copy
    _src = params[0];
    _dest = params[1];
    current_dir = shell.get_current_dir();
    std::cout << "Current dir: " << shell.get_current_dir()->get_name() << std::endl;
    _execute();
}

// Ls Command
void LsCommand::_execute() {
    _list_directory(current_dir);
}

void LsCommand::_list_directory(const Directory* dir) const {
    for (auto iter = dir->fbegin(); iter != dir->fend(); ++iter) {
        auto file = *iter;
        std::cout << file->get_symbol() << "  " << file->get_name() << std::endl;
    }

    for (const auto& entry: *dir) {
        std::cout << dir->get_symbol() << "  " << entry->get_name() << std::endl;
    }

    if (recursive) { // recurse through directories
        for (const auto& entry: *dir) {
            std::cout << std::endl << entry->get_name() << ":\n";
            _list_directory(entry); // recursion
        }
    }
}

void LsCommand::execute(Shell& shell, const bool recursive) {
    this->recursive = recursive;
    current_dir = shell.get_current_dir();
    _execute();
}

void LsCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    if (params.size() >= 1) {
        bool recursive = params[0] == "-R";
    }
    current_dir = shell.get_current_dir();
    _execute();
    recursive = false; // clear
}


// Cat Command

CatCommand::CatCommand() {}

void CatCommand::_execute() {
    const File* file = nullptr;

    std::cout << "search dir: " << current_dir->get_full_path() << std::endl;
    std::cout << "search name: " << fname << std::endl;
    for (auto iter = current_dir->fbegin(); iter != current_dir->fend(); ++iter) {
        auto cfile = *iter;
        std::cout << "loop file: " << cfile->get_name()<< std::endl;
        if (cfile->get_name() == fname) {
            file = cfile;
        }
    }

    if (file == nullptr){
        throw std::invalid_argument("File not found: " + fname);
    }
    else
        std::cout << "Cat file: " << file->get_name() << std::endl;

    // dump characters through the File iterator
    /*for (auto &&i : *file) {
        std::cout << i;
    }
    */

   file->dump();
    for (auto it = file->begin(); it != file->end(); ++it) {
        std::cout << *it;
    }

    std::cout << std::endl;
}

void CatCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    fname = params[0];
    current_dir = shell.get_current_dir();
    _execute();
}

// TODO Implement Cd
// TODO Implement Mkdir
// TODO Implement rm
// TODO Implement link