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

};

#endif