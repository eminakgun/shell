#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "file.hpp"
#include "directory.hpp"

const size_t SIZE_10MB = 1024*1024*10;

class MemoryManager {
private:    
    static MemoryManager* instance;
    std::size_t total_size;

    // hide the constructor for singleton implmentation 
    MemoryManager();
    bool can_allocate(size_t size);

public:
    static MemoryManager* get(); // returns the singleton object
    
    // Alloc/Dealloc
    File*        allocate_file(const std::string& name, const std::string& content);
    Directory*   allocate_directory(const std::string& name, const std::string& path);

    std::map<std::string, File*> entries;
};

#endif