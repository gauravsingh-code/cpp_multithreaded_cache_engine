cmake_minimum_required(VERSION 3.10)
project(cpp_multithreaded_cache_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cpp_multithreaded_cache_engine
    src/main.cpp
    src/cache/CacheEngine.cpp
    src/storage/StorageEngine.cpp
    src/command/CommandParser.cpp
    
    src/cache/CacheEngine.h
    src/storage/StorageEngine.h
    src/command/Command.h
    src/command/CommandParser.h
    )

target_include_directories(cpp_multithreaded_cache_engine PRIVATE src)

add_executable(cpp_multithreaded_cache_engine_tests
    tests/ThreadTest.cpp
    src/storage/StorageEngine.cpp
)
target_include_directories(cpp_multithreaded_cache_engine_tests PRIVATE src)