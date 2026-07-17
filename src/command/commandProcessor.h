#pragma once

#include <string>
#include "cache/CacheEngine.h"

class commandProcessor{
    public:
        commandProcessor(CacheEngine &cache_);
        ~commandProcessor();

    private:
    CacheEngine cache_;
    


};