#include "memory_manager.hpp"
#include <system_error>


namespace shell {

// static initialization
MemoryManager* MemoryManager::instance = nullptr;

//template<size_t SIZE_LIM>
MemoryManager::MemoryManager() {
    total_size = 0;
}

bool MemoryManager::can_allocate(size_t size) {
    if (total_size + size > SIZE_10MB)
        return false;
    else 
        return true;
}

MemoryManager* MemoryManager::get() {
    if (instance == nullptr) {
        instance = new MemoryManager();
    }
    return instance;
}

File* MemoryManager::allocate_file(const std::string& name, const std::string& content) {
    File* new_file = nullptr;
    auto it = entries.find(name);
    if (it == entries.end()) { // entry not found
        if (can_allocate(content.size())) {
            // create file object
            new_file = new File(name, content);

            // push it to our memory array
            entries[name] = new_file;

            // update allocated memory size
            total_size += new_file->size();
            std::cout << "Total size: " << total_size << std::endl;
        }
        else {
            throw std::system_error(std::make_error_code(std::errc::not_enough_memory),
                                    "Reached 10MB capacity!! Delete some file first");
        }
    }
    return new_file;
}

File* MemoryManager::allocate_symfile(const std::string& name, const File* link) {
    // since its only a shortcut, 
    //consider it does not occupy space on harddisk
    auto new_file = new SymFile(name, link);
    entries[name] = new_file;
    return new_file;
}

Directory* MemoryManager::allocate_directory(const std::string& name, const std::string& path) {
    Directory* dir = nullptr;
    auto it = entries.find(name);
    if (it == entries.end()) { // entry not found
        // create directory object
        dir = new Directory(name, path);

        entries[name] = dir;

        // update allocated memory size
        total_size += sizeof(dir);
        //std::cout << "Directory size: " << sizeof(dir) << std::endl;
        //std::cout << "Total size: " << total_size << std::endl;
    }
    return dir;   
}

void MemoryManager::deallocate(File* file) {
    auto it = entries.find(file->get_name());
    if (it != entries.end()) { // file found
        File* f = it->second;
        total_size -= sizeof(f);
        delete f;
        entries.erase(it);
    }
}

void MemoryManager::list_entries() {
    std::cout << "MemoryManager::list_entries" << std::endl;
    for (const auto &i : entries) {
        File* file = i.second;
        std::cout << file->get_symbol() << "  " 
                  << file->get_date()   << "  " 
                  << file->get_name() << std::endl;
    }
}

} // namespace
