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

void CpCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    // Copy
    _src = mount_path + "/" + params[0];
    _dest = params[1];
    current_dir = shell.get_current_dir();
    std::cout << "Current dir: " << shell.get_current_dir()->get_name() << std::endl;
    _execute();
}

void CpCommand::_execute() {
    // TODO check if source is outside of our file system
    // check if it exists
    // TODO Add directory copy

    std::cout << _src << std::endl;
    std::string content = FileSystemHandler::read_file(_src);

    std::cout << "CpCommand current dir: " << current_dir->get_name() << std::endl;
    std::cout << "file content: " << content << std::endl;

    Directory* found_dir = current_dir;
    auto target = FileSystemHandler::split_path(_dest); // path/to/target
    std::cout << "target begin: " << *target.begin() << std::endl;
    if (target.size() > 1) {
        auto search_dir = current_dir;
        Directory* target_dir = nullptr;
        for (auto &dir : target) {
            std::cout << "1st loop: " << dir << std::endl;
            std::cout << "1st loop: " << *(target.end()) << std::endl;
            if (dir == target[target.size()-1]) {
                // reached last element, it must be the file name
                break;
            }
            
            std::cout << dir << std::endl;
            target_dir = nullptr;
            for (const auto& ddir: *(search_dir)) { // Use directory iterator
                if (ddir->get_name() == dir) {
                    target_dir = const_cast<Directory*>(ddir);
                    search_dir = target_dir;
                    break;
                }
            }
        }
        if (target_dir == nullptr) 
            throw std::invalid_argument("Path not found: " + _dest);
        
        found_dir = target_dir;
    }
    
    std::cout << "Copying file to path: " << found_dir->get_name() << std::endl;
    // request a file from memory_manager and load it
    File* file = MemoryManager::get()->allocate_file(_dest, content);
    found_dir->add_file(file);

}

// Ls Command
void LsCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    if (params.size() >= 1) {
        bool recursive = params[0] == "-R";
    }
    current_dir = shell.get_current_dir();
    _execute();
    recursive = false; // clear
}

void LsCommand::execute(Shell& shell, const bool recursive) {
    this->recursive = recursive;
    current_dir = shell.get_current_dir();
    _execute();
    this->recursive = false;
}

void LsCommand::_execute() {
    _list_directory(current_dir);
}

void LsCommand::_list_directory(const Directory* dir) const {
    std::cout << "_list_directory" << dir->get_name() << std::endl;
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

// Cat Command

CatCommand::CatCommand() {}

void CatCommand::_execute() {
    File* file = nullptr;

    std::cout << "search dir: " << current_dir->get_full_path() << std::endl;
    std::cout << "search name: " << fname << std::endl;
    for (auto iter = current_dir->fbegin(); iter != current_dir->fend(); ++iter) {
        auto cfile = *iter;
        if (cfile->get_name() == fname) {
            file = const_cast<File*>(cfile);
            break;
        }
    }

    if (file == nullptr){
        throw std::invalid_argument("File not found: " + fname);
    }
    else
        std::cout << "Cat file: " << file->get_name() << std::endl;

    auto sym_file = dynamic_cast<const SymFile*>(file);
    if (sym_file != nullptr) {
        auto link = sym_file->get_link();
        if (link != nullptr){
            for (const auto &i : *link)
                std::cout << i;
        }
    }
    else {
        for (auto it = file->begin(); it != file->end(); ++it)
            std::cout << *it;
    }

    std::cout << std::endl;
}

void CatCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    fname = params[0];
    current_dir = shell.get_current_dir();
    _execute();
}


void CdCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    dname = params[0];
    found_dir = nullptr;
    current_dir = shell.get_current_dir();
    _execute();

    if (found_dir != nullptr) {        
        shell.set_current_dir(found_dir);
    }
}

void CdCommand::_execute() {
    //std::cout << "CdCommand::_execute " << std::endl;
    //std::cout << "search dir: " << current_dir->get_full_path() << std::endl;
    //std::cout << "search name: " << dname << std::endl;

    if (dname == "."){
        /* stays the same */
    }
    else if (dname == "..") {
        found_dir = current_dir->get_parent();
    }
    else {
        auto target = FileSystemHandler::split_path(dname); // path/to/target
        auto search_dir = current_dir;
        Directory* target_dir = nullptr;
        for (auto &dir : target) {
            std::cout << dir << std::endl;
            target_dir = nullptr;
            for (const auto& ddir: *(search_dir)) { // Use directory iterator
                if (ddir->get_name() == dir) {
                    target_dir = const_cast<Directory*>(ddir);
                    search_dir = target_dir;
                    break;
                }
            }
            if (target_dir == nullptr) 
                break;
            
        }
        found_dir = target_dir;

        if (found_dir == nullptr){
            throw std::invalid_argument("Directory not found: " + dname);
        } else
            std::cout << "Cd file: " << found_dir->get_name() << std::endl;
    }
}

void RmCommand::execute(Shell& shell, const std::vector<std::string>& params){
    fname = params[0];
    current_dir = shell.get_current_dir();
    _execute();
}
void RmCommand::_execute(){
    // delete the given file in current directory
    File* file;
    for (auto iter = current_dir->fbegin(); iter != current_dir->fend(); ++iter) {
        auto cfile = *iter;
        if (cfile->get_name() == fname) {
            file = const_cast<File*>(cfile);
            break;
        }
    }
    current_dir->delete_file(file);
    MemoryManager::get()->deallocate(file);
    // TODO immediately remove from disk as well
}


void LinkCommand::execute(Shell& shell, const std::vector<std::string>& params){
    _src = params[0];
    _dest = params[1];
    current_dir = shell.get_current_dir();
    _execute();
};

void LinkCommand::_execute() {
    // Find source file
    File* sfile;
    for (auto iter = current_dir->fbegin(); iter != current_dir->fend(); ++iter) {
        sfile = const_cast<File*>(*iter);
        if (sfile->get_name() == _src)
            break;
    }

    MemoryManager* mm = MemoryManager::get();
    auto symfile = mm->allocate_symfile(_dest, sfile);
    current_dir->add_file(symfile);
};


void MkdirCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    dname = params[0];
    current_dir = shell.get_current_dir();
    _execute();
}

void MkdirCommand::_execute() {
    // TODO Check existence of given dname, delete entry if exists
    //      immediately remove from disk as well
    std::string full_path = current_dir->get_full_path();
    MemoryManager* mm = MemoryManager::get();
    Directory* alloc = mm->allocate_directory(dname, full_path + "/" + dname);
    current_dir->add_subdir(alloc);
}


// Utility
void MMCommand::_execute() {
    MemoryManager::get()->list_entries();
}