#include "directory.hpp"

void Directory::add_file(File* file) {
    files.push_back(file);
}

void Directory::add_subdir(Directory* dir) {
    childs.push_back(dir);
}