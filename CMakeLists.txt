cmake_minimum_required(VERSION 3.10)
project(cpp_multithreaded_cache_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0600)
endif()

add_executable(cpp_multithreaded_cache_engine
    src/main.cpp
    src/cache/CacheEngine.cpp
    src/storage/StorageEngine.cpp
    src/command/CommandParser.cpp
    src/network/TcpServer.cpp
    
    src/cache/CacheEngine.h
    src/storage/StorageEngine.h
    src/command/Command.h
    src/command/CommandParser.h
    src/network/TcpServer.h
    )

target_include_directories(cpp_multithreaded_cache_engine PRIVATE src)
target_link_libraries(cpp_multithreaded_cache_engine PRIVATE Ws2_32)

add_executable(cpp_multithreaded_cache_engine_tests
    tests/ThreadTest.cpp
    src/storage/StorageEngine.cpp
)
target_include_directories(cpp_multithreaded_cache_engine_tests PRIVATE src)

add_executable(cache_client 
    src/client/Client.cpp
)
target_include_directories(cache_client PRIVATE src)
target_link_libraries(cache_client PRIVATE Ws2_32)
