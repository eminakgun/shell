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

    // Iterator Interface for the file content
    class Iterator {
    private:
        const std::string& content;
        size_t current;

    public:
        Iterator(const std::string& content) : content(content), current(0) {}
        Iterator(const std::string& content, size_t current) : content(content), current(current) {}

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        char operator*() const {
            return content[current];
        }

        Iterator& operator++() {
            ++current;
            return *this;
        }
    };

    // Functions to get iterators for content
    Iterator begin() const {
        return Iterator(content);
    }

    Iterator end() const {
        return Iterator(content, content.size() - 1);
    }
};

#endif