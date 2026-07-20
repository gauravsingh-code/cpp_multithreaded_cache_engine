#pragma once

#include <string>
#include "cache/CacheEngine.h"

class commandProcessor{
    public:
        commandProcessor(CacheEngine &cache_);
        ~commandProcessor();

        std::string execute(const std::string &input);

    private:
    CacheEngine &cache_;    

};