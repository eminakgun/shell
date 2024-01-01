#include "file.hpp"

File::File(const std::string& name) 
            : symbol("F"), name(name), date(get_timestamp()), content("") {}

File::File(const std::string& name, std::string& content) 
            : symbol("F"), name(name), date(get_timestamp()), content(content) {}

// TODO Refactor into a separate class function
std::string File::get_timestamp() {
    // Convert the current time to a tm structure
    std::time_t currentTime = std::time(nullptr);
    std::tm* timeInfo = std::localtime(&currentTime);
    
    // Format the date and time
    std::stringstream ss;
    ss << std::put_time(timeInfo, "%b %d %H:%M");

    return ss.str();
}

const std::string& File::get_name() const {
    return name;
}

const std::string& File::get_content() const {
    return content;
}

const void File::dump() const {
    // i.e cat command
    std::cout << content << std::endl;
}

std::ostream& operator<<(std::ostream& os, const File& f) {
    os << f.symbol << " " << f.name << "    " 
        << f.date << "    " << f.content.size();
    return os;
}