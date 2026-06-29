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

     std::size_t size() const;

     private:
     std::unordered_map<
     std::string,
     Entry
     > store__;

     std::size_t size();
};