#include "shell.hpp"

Shell::Shell() : current_dir(Directory("")) {
    commands["exit"] = new ExitCommand();
    commands["cd"] = nullptr;
    commands["mkdir"] = nullptr;
    commands["cp"] = new CpCommand();
    commands["ls"] = new LsCommand();
    commands["cat"] = new CatCommand();
    boot();
}

void Shell::boot() {
    memory_manager = MemoryManager::get();

    // TODO check disk existence
    // TODO Include date information as well

    root["root"] = &current_dir;

    // Iterate over the files and folders in the disk folder
    // and load it into memory space
    load_from_directory(root["root"]);

    // list recursively
    dynamic_cast<LsCommand*>(commands["ls"])->execute(*this, true);
}

void Shell::load_from_directory(Directory* dir) {
    std::string dir_path = dir->get_full_path();
    
    std::cout << "Enter dir: " << dir->get_name() << std::endl;
    vector<std::string> files = FileSystemHandler::list_files(mount + dir_path);

    for (const auto &entry : files){
        std::cout << "Allocate file: " << entry << std::endl;
        std:string content = FileSystemHandler::read_file(mount + dir_path + "/" + entry);
        std::cout << "Content: " << content << std::endl;
        File* alloc = memory_manager->allocate_file(entry, content);
        dir->add_file(alloc);
    }

    for (const auto& entry : FileSystemHandler::list_directories(mount + dir_path)){
        std::cout << "Allocate folder: " << entry << std::endl;
        Directory* alloc = memory_manager->allocate_directory(entry, dir_path + "/" + entry);
        dir->add_subdir(alloc);
        load_from_directory(alloc);
    }

}

void Shell::add_command(const std::string& name, Command* cmd) {
    commands[name] = cmd;
}

void Shell::execute_command(Command& cmd) {
    cmd.execute(*this);
}

void Shell::execute_command(const std::string& input) {
    auto parsed_cmd = CommandParser::parse(input);
    auto it = commands.find(parsed_cmd.first); // get iterator

    if (it != commands.end() && it->second != nullptr) {            
        it->second->execute(*this, parsed_cmd.second);
    } else {
        std::cout << "Command not found: " << parsed_cmd.first << std::endl;
    }
    
    
}

void Shell::flush() {
    
    for (auto iter = root["root"]->fbegin(); iter != root["root"]->fend(); ++iter) {
        auto file = *iter;
        if ("F" == file->get_symbol()) {
            std::string path = mount + "/" + file->get_name();
            FileSystemHandler::write_file(file, mount + "/" + file->get_name());
        }
    }

    for (const auto& dir: *(root["root"])) { // Use directory iterator
        const std::string& sym = dir->get_symbol();
        if ("D" == sym) {
            std::cout << "Flush directory: " << dir->get_name() << std::endl;
            // TODO Refactor so that flush directories as well.
        }
    }

}

File* Shell::find_file(const std::string& path) {
    File* file = nullptr;
    char delimiter = '/';
    std::string tmp = "";
    std::vector<std::string> parts;

    // refactor a function
    std::istringstream iss(path);
    while(std::getline(iss, tmp, delimiter)) {
        parts.push_back(tmp);
    }
    
    std::cout << "splitted path: " << std::endl;
    for (auto &&i : parts)
    {
        std::cout << i << ", ";
    }
    std::cout << std::endl;
    
    return file;
}