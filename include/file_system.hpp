#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>

// OS package
#include <dirent.h> // for directory search

using std::string;
using std::vector;

#include "file.hpp"


// TODO
// - move func implementations to .cpp file
// - further replace with my own file I/O

// FileSystemHandler class provides the interface
// for directory/file operations such as reading/writing
// Currently uses regular OS tasks
class FileSystemHandler {
public:
    static vector<string> list_files(const string& folder_path) {
        vector<string> files;

        // Reference https://stackoverflow.com/questions/13129340/recursive-function-for-listing-all-files-in-sub-directories
        if (auto dir = opendir(folder_path.c_str())) {
            while (auto f = readdir(dir)) {
                auto dir_name = std::string(f->d_name);
                auto d_type = f->d_type;
                if (d_type == DT_REG)
                    files.push_back(dir_name);
            }
        }

        if (files.empty())
            throw std::invalid_argument("No files found in directory: " + folder_path);
        
        return files;
    }

    static string read_file(const string& file_path) {
        std::ifstream src_file(file_path);
        if (!src_file.is_open()) {
            throw std::invalid_argument("Error opening input file: " + file_path);
        }

        // read _src file into a string
        std::string content((std::istreambuf_iterator<char>(src_file)),
                                std::istreambuf_iterator<char>());
        src_file.close(); // Close the file
        return content;
    }

    static void write_file(const File* file, const string&& file_path) {
        std::ofstream ofs(file_path);
        if (!ofs.is_open()) {
            throw std::invalid_argument("Error opening input file: " + file_path);
        }

        std::cout << "Flush file: " << file->get_name() << std::endl;
        ofs << file->get_content();
        ofs.close();
    }


    static vector<string> list_directories(const string& folder_path) {
        vector<string> dirs;

        if (auto dir = opendir(folder_path.c_str())) {
            while (auto f = readdir(dir)) {
                auto dir_name = std::string(f->d_name);
                auto d_type = f->d_type;
                if (dir_name != "." && dir_name != "..") {
                    if (d_type == DT_DIR)
                        dirs.push_back(dir_name);
                }
            }
        }
        
        return dirs;
    }

};



#endif
