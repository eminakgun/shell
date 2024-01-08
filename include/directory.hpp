#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <vector>

#include "file.hpp"

class Directory : public File
{
private:
    Directory* parent;
    std::vector<File*> files;
    std::vector<Directory*> childs;
    std::string full_path;

public:
    Directory(const std::string& name) 
        : File(name), full_path(""), parent(nullptr)  {symbol = "D";}
    Directory(const std::string& name, const std::string& full_path) 
        : File(name, "D"), full_path(full_path) , parent(nullptr) {symbol = "D";};
    
    void add_file(File* file);
    void add_subdir(Directory* dir);
    void delete_file(File* file);
    void delete_dir(Directory* dir);


    std::string get_full_path() const {return full_path;}
    Directory* get_parent() {return parent;}
    File* get_file(std::string& name) const;

    // Iterator Interface
    class Iterator {
        private:
            typename std::vector<Directory*>::const_iterator current;

        public:
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