#include "memory_manager.hpp"

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
    // TODO check existence of the same file

    if (can_allocate(content.size())) {
        // create file object
        File* new_file = new File(name, content);

        // push it to our memory array
        entries[name] = new_file;

        // update allocated memory size
        total_size += new_file->size();
        std::cout << "Total size: " << total_size << std::endl;
        return new_file;
    }
}

Directory* MemoryManager::allocate_directory(const std::string& name, const std::string& path) {
    // TODO check existence of the same directory?

    // create directory object
    Directory* dir = new Directory(name, path);

    entries[name] = dir;

    // update allocated memory size
    total_size += sizeof(dir);
    std::cout << "Directory size: " << sizeof(dir) << std::endl;
    std::cout << "Total size: " << total_size << std::endl;
    return dir;   
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