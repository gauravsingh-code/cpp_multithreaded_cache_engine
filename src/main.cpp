#include<iostream>

#include "cache/CacheEngine.h"

int main(){

    CacheEngine cache;

    cache.set("name", "raghav");
    cache.set("city", "ahmedabad");

    auto name = cache.get("city");

    if(name){
        std::cout<<*name<<"\n";
    }

    cache.erase("city");

     auto city = cache.get("city");

    if(!city)
    {
        std::cout << "Not Found\n";
    }

    return 0;
}