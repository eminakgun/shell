#ifndef FILE_HPP
#define FILE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

// Abstract base class
class File {
protected:
    std::string symbol;
    std::string name;
    std::string date;
    std::string content;

    std::string get_timestamp();

public:
    File(const std::string& name);
    File(const std::string& name, const std::string& content);

    const std::string& get_name() const;
    const std::string& get_symbol() const {return symbol;}
    const std::string& get_content() const;
    const void dump() const;
    friend std::ostream& operator<<(std::ostream& os, const File& f);

    virtual size_t size() {return content.size();}
};

#endif