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

void MemoryManager::allocate_file(std::string& name, std::string& content) {
    if (can_allocate(content.size())) {
        // create file object
        File* new_file = new File(name, content);

        // push it to our memory array
        root.push_back(new_file);

        // update allocated memory size
        total_size += new_file->size();
        std::cout << "Total size: " << total_size << std::endl;
    }
}

void MemoryManager::allocate_directory(std::string& path, std::string& name) {
    if (path == "") {
        // root path
        // create directory object
        Directory* dir = new Directory(name);

        root.push_back(dir);

        // update allocated memory size
        total_size += sizeof(dir);
        std::cout << "Directory size: " << sizeof(dir) << std::endl;
        std::cout << "Total size: " << total_size << std::endl;
    }
    else {
        // search given path
    }
    
}