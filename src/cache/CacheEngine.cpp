#include "CacheEngine.h"

bool CacheEngine::set(
    const std::string &key,
    const std::string &value
){
    return storage__.set(key,value);
}

std::optional<std::string>CacheEngine::get(
    const std::string &key
){
    return storage__.get(key);
}

bool CacheEngine::erase(
    const std::string &key
){
    return storage__.erase(key);
}

