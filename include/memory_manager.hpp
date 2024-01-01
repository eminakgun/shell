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
    void   allocate_file(std::string& name, std::string& content);
    void   allocate_directory(std::string& path, std::string& name);

    std::vector<File*> root;


    // TODO Seperate iterators for directories and files
    // Iterator class
    class Iterator {
    private:
        std::vector<File*>::iterator current;

    public:
        Iterator(std::vector<File*>::iterator iter) : current(iter) {}

        // Overload operators to support iterator functionality
        File* operator*() const {
            return *current;
        }

        Iterator& operator++() {
            ++current;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() {
        return Iterator(root.begin());
    }

    Iterator end() {
        return Iterator(root.end());
    }
};

#endif