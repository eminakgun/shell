#include "shell.hpp"

namespace shell {

Shell::Shell() : root_dir(Directory("root")) {
    commands["exit"] = new ExitCommand();
    commands["cp"] = new CpCommand();
    commands["ls"] = new LsCommand();
    commands["cat"] = new CatCommand();
    commands["cd"] = new CdCommand();
    commands["mkdir"] = new MkdirCommand();
    commands["link"] = new LinkCommand();
    commands["rm"] = new RmCommand();
    commands["mm"] = new MMCommand();
    commands["flush"] = new FlushCommand();
    boot();
}

void Shell::boot() {
    memory_manager = MemoryManager::get();

    // TODO check disk existence
    // TODO Include date information as well
    
    current_dir = &root_dir;

    // Iterate over the files and folders in the disk folder
    // and load it into memory space
    load_from_directory(&root_dir);

    const char* banner = "===============================\n"
                         "   _____ __         ____\n"
                         "  / ___// /_  ___  / / /\n"
                         "  \\__ \\/ __ \\/ _ \\/ / /\n"
                         " ___/ / / / /  __/ / /\n"  
                         "/____/_/ /_/\\___/_/_/\n"
                         "===============================\n"
                         "Boot completed successfully!\n";

    std::cout << banner << "\t\t\t" << std::endl;
                        
    // list recursively
    std::cout << ">ls -R" << std::endl;
    dynamic_cast<LsCommand*>(commands["ls"])->execute(*this, true);
}

void Shell::load_from_directory(Directory* dir) {
    std::string dir_path = dir->get_full_path();
    
    std::cout << "Enter dir: " << dir->get_name() << std::endl;
    vector<std::string> files = fs.api.list_files(mount_path + dir_path);

    for (const auto &entry : files){
        std::cout << "Allocate file: " << entry << std::endl;
        std:string content = fs.api.read_file(mount_path + dir_path + "/" + entry);
        std::cout << "Content: " << content << std::endl;
        File* alloc = memory_manager->allocate_file(entry, content);
        dir->add_file(alloc);
    }

    for (const auto& entry : fs.api.list_symlink(mount_path + dir_path)){
        std::cout << "Allocate file: " << entry << std::endl;
        auto linked_file_name = fs.api.read_symlink(mount_path + dir_path + "/" + entry);
        std::cout << "linked_file_name: " << linked_file_name << std::endl;
        File* alloc = memory_manager->allocate_symfile(entry, dir->get_file(linked_file_name));
        dir->add_file(alloc);
    }

    for (const auto& entry : fs.api.list_directories(mount_path + dir_path)){
        std::cout << "Allocate folder: " << entry << std::endl;
        Directory* alloc = memory_manager->allocate_directory(entry, dir_path + "/" + entry);
        dir->add_subdir(alloc);
        load_from_directory(alloc);
    }

}

void Shell::interactive() {
    while (true) {
        std::cout << current_dir->get_name() << " > ";
        std::string input;
        std::getline(std::cin, input);
        try {
            execute_command(input);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            if ("exit -f" == input)
                exit(0);
        }
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
    //std::cout << current_dir->get_name() << " > " << input << std::endl;
    if (it != commands.end() && it->second != nullptr) {            
        it->second->execute(*this, parsed_cmd.second);
    } else {
        std::cout << "Command not found: " << parsed_cmd.first << std::endl;
    }
}

void Shell::flush() {
    _flush(&root_dir);
}

void Shell::_flush(const Directory* dir) {
    for (auto iter = dir->fbegin(); iter != dir->fend(); ++iter) {
        auto file = *iter;
        std::string path = mount_path + dir->get_full_path() + "/" + file->get_name();
        std::cout << "Full file path: " << path << std::endl;
        if ("F" == file->get_symbol()) {
            fs.api.write_file(file, path);
        }
        else { // SymFile
            auto link_file = dynamic_cast<SymFile*>(const_cast<File*>(file))->get_link();
            fs.api.create_symlink(link_file->get_name(), path);
        }
    }

    for (const auto& subdir: *(dir)) { // Use directory iterator
        const std::string& sym = subdir->get_symbol();
        if ("D" == sym) {
            std::cout << "Flush directory: " << subdir->get_full_path() << std::endl;
            fs.api.create_directory(mount_path + "/" + subdir->get_full_path());
            _flush(subdir);
        }
    }
}

} // namespace