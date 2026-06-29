#include "CommandParser.h"
#include<sstream>
#include<vector>

Command CommandParser::parse(const std::string &input){
    std::stringstream ss(input);
    std::vector<std::string>tokens;

    std::string token;

    while(ss >> token){
        tokens.push_back(token);
    }

    if(tokens.empty()){
        return {CommandType::INVALID, "", ""};
    }
    if(tokens[0] == "EXIT"){
        return {CommandType::EXIT, "", ""};
    }

    if(tokens[0] == "GET"){
        if(tokens.size() != 2){
            return {CommandType::INVALID, "", ""};
        }
        return {
            CommandType::GET,
            tokens[1],
            ""
        };
    }

    if(tokens[0] == "DELETE"){
        if(tokens.size() != 2){
            return {CommandType::DELETE_KEY, "", ""};
        }

        return {
            CommandType::INVALID,
            tokens[1],
            ""
        };
    }

    if(tokens[0] == "SET"){
        if(tokens.size() != 3){
            return {
                CommandType::INVALID,
                "",
                ""
            };
        }

        return {
            CommandType::SET,
            tokens[1],
            tokens[2]
        };
    }
    return {CommandType::INVALID, "", ""};
}