#include "commands.hpp"
#include "shell.hpp"

#include <unistd.h>

namespace shell {

// Command Implementations
void Command::execute(Shell& shell) {
    current_dir = shell.get_current_dir();
    _execute();
}

// Exit Command
void ExitCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    flush = true;
    if (params.size() >= 1) {
        if (params[0] == "-f")
            flush = false;
    }
    
    if (flush)
        shell.flush();
    _execute();
}

void ExitCommand::_execute() {
    std::cout << "Exiting the shell." << std::endl;
    exit(0);
}


// Cp Command
CpCommand::CpCommand(){}
CpCommand::CpCommand(std::string src, std::string dest) : _src(src), _dest(dest) {}

void CpCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    // Copy
    //_src = mount_path + "/" + params[0];
    _src = params[0];
    _dest = params[1];
    current_dir = shell.get_current_dir();
    //std::cout << "Current dir: " << shell.get_current_dir()->get_name() << std::endl;
    _execute();
}
        
void CpCommand::_execute() {
    // TODO Add directory copy
    std::cout << _src << std::endl;
    std::string content;
    if (current_dir->get_name() == "root" && 
            fs.split_path(_src)[0] == "..") {
        /* Means we need to access outside of our mount point */
        _src = mount_path + "/" + _src;
        content = fs.api.read_file(_src);
    }
    else {
        std::pair<Directory*, File*> deepest = Directory::get_deepest(current_dir, _src);
        if (deepest.second == nullptr)
            throw std::invalid_argument("Path not found: " + _src);
        content = deepest.second->get_content();
        
    }

    //std::cout << "CpCommand current dir: " << current_dir->get_name() << std::endl;
    //std::cout << "file content: " << content << std::endl;

    // TODO Refactor recursive path search into a function, 
    //      return deepest directory and file name

    std::vector<std::string> path = fs.split_path(_dest);
    std::string dest_name = path[path.size()-1];
    std::pair<Directory*, File*> dest_deepest = Directory::get_deepest(current_dir, _dest);
    
    if (dest_deepest.second == nullptr) {
        std::cout << "Copying file to path: " << dest_deepest.first->get_name() << std::endl;
        // request a file from memory_manager and load it
        File* file = MemoryManager::get()->allocate_file(dest_name, content);
        dest_deepest.first->add_file(file);
    }
    else {
        throw std::invalid_argument("Path already exists: " + _dest);
    }
}

// Ls Command
void LsCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    if (params.size() >= 1) {
        recursive = params[0] == "-R";
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
    //std::cout << "_list_directory: " << dir->get_name() << std::endl;
    for (auto iter = dir->fbegin(); iter != dir->fend(); ++iter) {
        auto file = *iter;
        std::cout << file->get_symbol() << "  " << file->get_name();
        if ("L" == file->get_symbol()) {
            auto link_file = dynamic_cast<SymFile*>(const_cast<File*>(file));
            std::cout << "-> " << link_file->get_link()->get_name();
        }
        std::cout << std::endl;
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
void CatCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    fname = params[0];
    current_dir = shell.get_current_dir();
    _execute();
}

void CatCommand::_execute() {

    //std::cout << "search dir: " << current_dir->get_full_path() << std::endl;
    //std::cout << "search name: " << fname << std::endl;

    Directory* found_dir = current_dir;
    auto target = fs.split_path(fname); // path/to/target
    if (target.size() > 1) {
        auto search_dir = current_dir;
        Directory* target_dir = nullptr;
        for (auto &dir : target) {
            if (dir == target[target.size()-1]) {
                // reached last element, it must be the file name
                fname = dir;
                break;
            }
            
            target_dir = nullptr;
            for (const auto& ddir: *(search_dir)) { // Use directory iterator
                if (ddir->get_name() == dir) {
                    target_dir = const_cast<Directory*>(ddir);
                    search_dir = target_dir;
                    break;
                }
            }
        }
        found_dir = target_dir;
    }

    if (found_dir == nullptr) // In case of hierarchical path
            throw std::invalid_argument("Path not found: " + fname);

    //std::cout << found_dir->get_name() << std::endl;

    // now search file
    File* file = nullptr;
    for (auto iter = found_dir->fbegin(); iter != found_dir->fend(); ++iter) {
        auto cfile = *iter;
        if (cfile->get_name() == fname) {
            file = const_cast<File*>(cfile);
            break;
        }
    }

    if (file == nullptr)
        throw std::invalid_argument("File not found: " + fname);
    else
        std::cout << "Cat file: " << file->get_name() << std::endl;

    // Dump file content
    auto sym_file = dynamic_cast<const SymFile*>(file);
    if (sym_file != nullptr) { // link file
        auto link = sym_file->get_link();
        if (link != nullptr){
            for (const auto &i : *link)
                std::cout << i;
        }
    }
    else { // regular file
        for (auto it = file->begin(); it != file->end(); ++it)
            std::cout << *it;
    }

    std::cout << std::endl;
}


// Cd Command
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
        auto target = fs.split_path(dname); // path/to/target
        auto search_dir = current_dir;
        Directory* target_dir = nullptr;
        for (auto &dir : target) {
            //std::cout << dir << std::endl;
            target_dir = nullptr;
            if (dir == "..") {
                search_dir = search_dir->get_parent();
                target_dir = search_dir;
            } 
            else {
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
            
        }
        found_dir = target_dir;

        if (found_dir == nullptr){
            throw std::invalid_argument("Directory not found: " + dname);
        } else
            std::cout << "Change directory: " << found_dir->get_name() << std::endl;
    }
}

void RmCommand::execute(Shell& shell, const std::vector<std::string>& params){
    fname = params[0];
    current_dir = shell.get_current_dir();
    _execute();
}

void RmCommand::_execute(){
    // TODO Add hierarchical path search

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
    std::string hard_path = mount_path + current_dir->get_full_path() + "/" + file->get_name();
    unlink(hard_path.c_str()); // TODO move into FileSystemHandler
    MemoryManager::get()->deallocate(file);  
}


// Link Command
void LinkCommand::execute(Shell& shell, const std::vector<std::string>& params){
    _src = params[0];
    _dest = params[1];
    current_dir = shell.get_current_dir();
    _execute();
};

void LinkCommand::_execute() {
    // TODO Add hierarchical path search

    // Find source file
    File* sfile;
    for (auto iter = current_dir->fbegin(); iter != current_dir->fend(); ++iter) {
        sfile = const_cast<File*>(*iter);
        if (sfile->get_name() == _src)
            break;
    }

    if (current_dir->get_file(_dest) == nullptr) {
        MemoryManager* mm = MemoryManager::get();
        auto symfile = mm->allocate_symfile(_dest, sfile);
        current_dir->add_file(symfile);
    }
    else
        throw std::invalid_argument("Soft link already exists: " + _dest);
};


void MkdirCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    dname = params[0];
    current_dir = shell.get_current_dir();
    _execute();
}

void MkdirCommand::_execute() {
    // TODO Add hiearhical path search

    MemoryManager* mm = MemoryManager::get();
    Directory* exists = nullptr;
    for (const auto& dir: *current_dir) { // Use directory iterator
        if (dname == dir->get_name())
            exists = const_cast<Directory*>(dir);
    }

    if (exists) {
        current_dir->delete_dir(exists);
        std::string hard_path = mount_path + "/" + exists->get_full_path();
        rmdir(hard_path.c_str()); // TODO move into FileSystemHandler
        mm->deallocate(exists);
    }
    else {
        std::string full_path = current_dir->get_full_path();
        Directory* alloc = mm->allocate_directory(dname, full_path + "/" + dname);
        current_dir->add_subdir(alloc);
    }
}


// Utilities

void MMCommand::_execute() {
    MemoryManager::get()->list_entries();
}

void FlushCommand::execute(Shell& shell, const std::vector<std::string>& params) {
    shell.flush();
}

} // namespace