#include "command_parser.hpp"


ParsedCmd CommandParser::parse(const std::string& input) {
    std::istringstream iss(input);
    std::string command;
    std::vector<std::string> parameters;

    iss >> command;
    std::string parameter;
    while (iss >> parameter) {
        parameters.push_back(parameter);
    }

    return std::make_pair(command, parameters);
}