#include "CommandProcessor.h"
#include "CommandParser.h"
#include "Command.h"

commandProcessor::commandProcessor(CacheEngine &cache_) : cache_(cache_) {}

std::string commandProcessor::execute(const std::string &input)
{
    // the first thing we do is to parse the input command
    CommandParser parser;

    // returns commandtype, key and value
    auto command = parser.parse(input);

    switch (command.type)
    {
    case CommandType::GET:
    {
        auto value = cache_.get(command.key);
        if (value)
        {
            std::cout << *value << '\n';
        }
        else
        {
            std::cout << "NOT FOUND\n";
        }
        break;
    }

    case CommandType::SET:
    {
        cache_.set(command.key, command.value);
        // std::cout<<"Key: "<<command.key<<" Value: "<<command.value<<"\n";
        std::cout << "OK\n";
        break;
    }

    case CommandType::DELETE_KEY:
    {
        bool isRemoved = cache_.erase(command.key);
        std::cout << (isRemoved ? "OK" : "NOT FOUND") << "\n";
        break;
    }
    case CommandType::ALL:
    {
        cache_.all();
        break;
    }
    case CommandType::EXIT:
    {
        return "EXIT";
    }
    case CommandType::INVALID:
    {
        std::cout<<"INVALID COMMAND"<<"\n";
        break;
    }
    default:
    {
        std::cout<<"INVALID COMMAND\n";
        std::cout<<"INPut: "<<input<<"\n";
    }
    }
}