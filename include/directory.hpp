#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <vector>

#include "file.hpp"

class Directory : public File
{
private:
    std::vector<File*> files;
    std::vector<Directory*> childs;
public:
    Directory(const std::string& name);
    //friend std::ostream& operator<<(std::ostream& os, const Directory& f);

    // Iterator Interface
    class Iterator {
        private:
            const std::vector<Directory*>& directories;
            typename std::vector<Directory*>::const_iterator current;

        public:
            Iterator(const std::vector<Directory*>& directories) 
                : directories(directories), current(directories.begin()) {}

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

    Iterator begin() const {
        return Iterator(childs);
    }

    Iterator end() const {
        //
    }
};

#endif