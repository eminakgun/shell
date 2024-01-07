#include "directory.hpp"

void Directory::add_file(File* file) {
    files.push_back(file);
}

void Directory::delete_file(File* file) {
    for (auto it = files.begin(); it != files.end(); ++it) {
        if ((*it)->get_name() == file->get_name()) {
            files.erase(it);  // Erase the file from the vector
        }
    }
}

void Directory::add_subdir(Directory* dir) {
    dir->parent = this;
    childs.push_back(dir);
}