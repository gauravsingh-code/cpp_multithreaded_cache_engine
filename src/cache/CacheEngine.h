#pragma once

#include <optional>
#include <string>

#include "../storage/StorageEngine.h"

class CacheEngine
{
public:
    bool set(
        const std::string &key,
        const std::string &value);

    std::optional<std::string> get(
        const std::string &key);

    bool erase(const std::string &key);

    void all();

private:
    StorageEngine storage__;
};