#include <utility>
#include <string>
#include <vector>
#include <sstream>

#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

namespace shell {

typedef std::pair<std::string, std::vector<std::string>> ParsedCmd;

class CommandParser {
public:
    // Returns command name as key
    // and command parameters as vector of string
    // based on passed input string
    static ParsedCmd parse(const std::string& input);
};

} // namespace

#endif