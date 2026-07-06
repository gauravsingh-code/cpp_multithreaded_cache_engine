#include <iostream>
#include <thread>
#include <vector>

#include "../src/storage/StorageEngine.h"

void worker(StorageEngine& storage, int id)
{
    for (int i = 0; i < 5; i++)
    {
        std::string key =
            "key_" +
            std::to_string(id) +
            "_" +
            std::to_string(i);

        std::string value =
            "value_" +
            std::to_string(i);

        storage.set(key, value);

        auto result = storage.get(key);

        if(result)
        {
            std::cout
                << "[Thread "
                << id
                << "] "
                << key
                << " = "
                << *result
                << '\n';
        }
    }
}

int main()
{
    StorageEngine storage;

    std::thread t1(worker, std::ref(storage), 1);
    std::thread t2(worker, std::ref(storage), 2);
    std::thread t3(worker, std::ref(storage), 3);

    t1.join();
    t2.join();
    t3.join();

    std::cout
        << "\nFinal Cache Size : "
        << storage.size()
        << '\n';

    return 0;
}