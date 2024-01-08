#include "directory.hpp"

void Directory::add_file(File* file) {
    files.push_back(file);
}

void Directory::delete_file(File* file) {
    for (auto it = files.begin(); it != files.end(); ++it) {
        if ((*it) == file) {
            files.erase(it);  // Erase the file from the vector
        }
    }
}

void Directory::delete_dir(Directory* dir) {
    for (auto it = childs.begin(); it != childs.end(); ++it) {
        if ((*it) == dir) {
            childs.erase(it);
            break;
        }
    }
}

void Directory::add_subdir(Directory* dir) {
    dir->parent = this;
    childs.push_back(dir);
}

File* Directory::get_file(std::string& name) const {
    File* found_file = nullptr;
    for (auto it = files.begin(); it != files.end(); ++it) {
        if (name == (*it)->get_name()) {
            found_file = (*it);
            break;
        }
    }
    return found_file;
}