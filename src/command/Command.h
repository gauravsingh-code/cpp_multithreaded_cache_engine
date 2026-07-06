#pragma once

#include<string>

enum class CommandType{
    SET,
    GET,
    DELETE_KEY,
    INVALID,
    EXIT,
    ALL
};

struct Command{
    CommandType type;
    std::string key;
    std::string value;
};





