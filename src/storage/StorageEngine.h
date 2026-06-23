#pragma once

#include<unordered_map>
#include<optional>
#include<string>

#include "Entry.h"

class StorageEngine
{
    public:
     bool set(
        const std::string &key,
        const std::string &value
     );

     std::optional<std::string> get(
        const std::string &key
     );

     bool erase(
        const std::string &key
     );


     private:
     std::unordered_map<
     std::string,
     Entry
     > store__;
};