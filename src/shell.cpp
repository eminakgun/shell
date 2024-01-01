
#include <vector>
#include <string>
#include <map>

#include "shell.hpp"

Shell::Shell() : current_dir(Directory("root")) {
    commands["exit"] = new ExitCommand();
    commands["cd"] = nullptr;
    commands["mkdir"] = nullptr;
    commands["cp"] = new CpCommand();
    boot();
}

void Shell::boot() {
    memory_manager = MemoryManager::get();

    // TODO check disk existence

    // Iterate over the files in the folder
    // and load it into memory space
    // reference https://stackoverflow.com/questions/13129340/recursive-function-for-listing-all-files-in-sub-directories
    if (auto dir = opendir(mount.c_str())) {
        while (auto f = readdir(dir)) {
            auto dir_name = std::string(f->d_name);
            auto d_type = f->d_type;
            if (dir_name != "." && dir_name != "..") {
                std::cout << "f: " << dir_name << std::endl;
                std::cout << "d_type: " << static_cast<int>(d_type) << std::endl;
                if (d_type == DT_REG) {
                    // Regular file
                    std::string path = mount + "/" + dir_name;
                    std::ifstream src_file(path);
                    if (!src_file.is_open()) {
                        std::cout << "Error opening input file: " << path << std::endl;
                        continue;
                    }

                    // read _src file into a string
                    std::string content((std::istreambuf_iterator<char>(src_file)),
                                            std::istreambuf_iterator<char>());
                    // TODO Include date information as well
                    memory_manager->allocate_file(dir_name, content);
                    src_file.close(); // Close the file

                }
                else if (d_type == DT_DIR) {
                    // TODO Include date information as well
                    std::string path = ""; // root for now
                    memory_manager->allocate_directory(path, dir_name);
                }
            }
        }
    }
    
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
    // TODO Refactor so that flushe directories as well.
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