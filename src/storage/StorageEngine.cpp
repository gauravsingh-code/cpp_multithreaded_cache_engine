#include "StorageEngine.h" //this works as interface and in .cpp files we are implementing the interface(Not fully interface)

bool StorageEngine::set(const std::string &key, const std::string &value){
    store__[key] = Entry{value};
    return true;
};

std::optional<std::string> StorageEngine::get(const std::string &key){
    auto it = store__.find(key);
    if(it ==  store__.end()){
        return std::nullopt;
    }
    return it->second.value;
};

bool StorageEngine::erase(const std::string &key){
    return store__.erase(key) > 0;
};

std::size_t StorageEngine::size() const{
    return store__.size();
};


