#include "directory.hpp"

namespace shell {

void Directory::add_file(File* file) {
    files.push_back(file);
}

void Directory::delete_file(File* file) {
    for (auto it = files.begin(); it != files.end(); ++it) {
        if ((*it) == file) {
            files.erase(it);  // Erase the file from the vector
            break;
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

std::pair<Directory*, File*> Directory::get_deepest(Directory* current_dir, std::string& path) {
    Directory* found_dir = current_dir;
    std::string fname = path;
    auto target = fs.split_path(path); // path/to/target
    if (target.size() > 1) {
        auto search_dir = current_dir;
        Directory* target_dir = nullptr;
        for (auto &dir : target) {
            std::cout << "dir: " << dir << std::endl;
            if (dir == target[target.size()-1]) {
                // reached last element, it must be the file name
                fname = dir;
                break;
            }
            
            target_dir = nullptr;
            if (dir == "..") {
                search_dir = search_dir->get_parent();
                target_dir = search_dir;
            }
            for (const auto& ddir: *(search_dir)) { // Use directory iterator
                if (ddir->get_name() == dir) {
                    target_dir = const_cast<Directory*>(ddir);
                    search_dir = target_dir;
                    break;
                }
            }
        }
        if (target_dir == nullptr) 
            throw std::invalid_argument("Path not found: " + fname);
        
        found_dir = target_dir;
    }
    
    return std::make_pair(found_dir, found_dir->get_file(fname));
}

} // namespace
