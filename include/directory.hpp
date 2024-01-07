#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <vector>

#include "file.hpp"

class Directory : public File
{
private:
    std::vector<File*> files;
    std::vector<Directory*> childs;
    std::string full_path;

public:
    Directory(const std::string& name) : File(name), full_path("") {symbol = "D";}
    Directory(const std::string& name, const std::string& full_path) 
        : File(name, "D"), full_path(full_path) {};
    
    void add_file(File* file);
    void add_subdir(Directory* dir);
    std::string& get_full_path() {return full_path;}

    // Iterator Interface
    class Iterator {
        private:
            //const std::vector<Directory*>& directories;
            typename std::vector<Directory*>::const_iterator current;

        public:
            // Iterator(const std::vector<Directory*>& directories) 
            //     : directories(directories), current(directories.begin()) {}
            
            Iterator(std::vector<Directory*>::const_iterator current) 
                : current(current) {}

            bool operator!=(const Iterator& other) const {
                return current != other.current;
            }

            const Directory* operator*() const {
                return *current;
            }

            Iterator& operator++() {
                ++current;
                return *this;
            }
        };

    class FileIterator {
        private:
            //const std::vector<File*>& files;
            std::vector<File*>::const_iterator current;

        public:
            FileIterator(std::vector<File*>::const_iterator current) 
                : current(current) {}
            
            bool operator!=(const FileIterator& other) const {
                return current != other.current;
            }

            const File* operator*() const {
                return *current;
            }

            FileIterator& operator++() {
                ++current;
                return *this;
            }
        };

    Iterator begin() const {
        return Iterator(childs.begin());
    }

    Iterator end() const {
        //
        return Iterator(childs.end());
    }

    FileIterator fbegin() const {
        return FileIterator(files.begin());
    }

    FileIterator fend() const {
        return FileIterator(files.end());
    }
};

#endif