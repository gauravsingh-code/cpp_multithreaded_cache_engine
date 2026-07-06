#include "StorageEngine.h" //this works as interface and in .cpp files we are implementing the interface(Not fully interface)
#include <mutex>
// #include <shared_mutex>

bool StorageEngine::set(const std::string &key, const std::string &value){
    std::unique_lock<std::shared_mutex> lock(mutex__);
    store__[key] = Entry{value};
    return true;
};

std::optional<std::string> StorageEngine::get(const std::string &key){
    std::shared_lock<std::shared_mutex> lock(mutex__);
    auto it = store__.find(key);
    if(it ==  store__.end()){
        return std::nullopt;
    }
    return it->second.value;
};

bool StorageEngine::erase(const std::string &key){
    //A shared_lock is used for read-only access, 
    // unique_lock is used for write access. 
    //Since erase modifies the map, we need to use a unique_lock to ensure exclusive access to the map during the erase operation.
    std::unique_lock<std::shared_mutex>lock(mutex__);
    return store__.erase(key) > 0;
};

std::size_t StorageEngine::size() const{
    std::shared_lock<std::shared_mutex> lock(mutex__);
    return store__.size();
};

void StorageEngine::all(){
    std::shared_lock<std::shared_mutex> lock(mutex__);
    for(const auto  &pair : store__){
        std::cout << "Key: " << pair.first << ", Value: " << pair.second.value << "\n";
    }
}


