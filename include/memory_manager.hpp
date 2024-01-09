#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "file.hpp"
#include "directory.hpp"

namespace shell {

const size_t SIZE_10MB = 1024*1024*10;

class MemoryManager {
private:    
    static MemoryManager* instance;
    std::size_t total_size;

    // TODO refactor so that multiple files with same name can exists
    std::map<std::string, File*> entries;

    // hide the constructor for singleton implmentation 
    MemoryManager();
    bool can_allocate(size_t size);

public:
    static MemoryManager* get(); // returns the singleton object
    
    void list_entries();
    
    // Alloc/Dealloc
    File*        allocate_file(const std::string& name, const std::string& content);
    File*        allocate_symfile(const std::string& name, const File* link);
    Directory*   allocate_directory(const std::string& name, const std::string& path);
    void         deallocate(File* file);
};

} // namespace

#endif