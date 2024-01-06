#include "shell.hpp"

Shell::Shell() : current_dir(Directory("root")) {
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

    // Iterate over the files and folders in the disk folder
    // and load it into memory space
    
    vector<std::string> files = FileSystemHandler::list_files(mount);
    std::cout << "FS list files:\n";
    for (const auto &entry : FileSystemHandler::list_files(mount)){
        std::cout << "Allocate file: " << entry << std::endl;
        //std::cout << "Content:" << FileSystemHandler::read_file(mount + "/" + i) << std::endl;
        std:string content = FileSystemHandler::read_file(mount + "/" + entry);
        memory_manager->allocate_file(entry, content);
    }

    for (const auto &entry : FileSystemHandler::list_directories(mount)){
        std::cout << "Allocate folder: " << entry << std::endl;
        std::string path = ""; // root for now
        memory_manager->allocate_directory(path, entry);
    }
    
    // print file system schema
    for (const auto file: memory_manager->root) {
        std::cout << "Loaded file: " << file->get_name() << std::endl;
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

void Shell::flush() const {
    // TODO Refactor so that flush directories as well.
    for (const auto file: memory_manager->root) {
        const std::string& sym = file->get_symbol();
        if ("F" == sym) {
            std::string path = mount + "/" + file->get_name();
            std::ofstream ofs(path);
            if (!ofs.is_open()) {
                std::cerr << "Error opening input file: " << path << std::endl;
                return;
            }

            ofs << file->get_content();
            ofs.close();
        }
        else if ("D" == sym) {
            
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
    

    for (auto &&i : memory_manager->root) {
        if (i->get_name() == path) {
            file = i;
        }
    }

    if (file == nullptr)
    {
        /* TODO Throw exception*/
    }
    
    return file;
}