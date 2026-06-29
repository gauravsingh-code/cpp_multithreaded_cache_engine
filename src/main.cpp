#include<iostream>

#include "cache/CacheEngine.h"
#include "command/CommandParser.h"

int main(){

    CacheEngine cache;
    CommandParser parser;

    std::string input;

    while(true){
        std::cout << "> ";

        std::getline(std::cin, input);

        auto command = parser.parse(input);

        switch(command.type){
            case CommandType::SET:
            {
                cache.set(command.key, command.value);
                std::cout<<"OK\n";
                break;
            }
            case CommandType::GET:{
                auto value = cache.get(command.key);
                if(value){
                    std::cout<<*value<<'\n';
                }
                else{
                    std::cout<<"NOT FOUND\n";
                }
                break;
            }

            case CommandType::DELETE_KEY:{
                bool removed = cache.erase(command.key);
                std::cout<<(removed ? "OK" : "NOT FOUND")<<"\n";
                break;
            }
            
            case CommandType::EXIT:{
                return 0;
            }

            default:
            {
                std::cout<<"INVALID COMMAND\n";
            }

        }

    }



    // CacheEngine cache;

    // cache.set("name", "raghav");
    // cache.set("city", "ahmedabad");

    // auto name = cache.get("city");

    // if(name){
    //     std::cout<<*name<<"\n";
    // }

    // cache.erase("city");

    //  auto city = cache.get("city");

    // if(!city)
    // {
    //     std::cout << "Not Found\n";
    // }

    return 0;
}