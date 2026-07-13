# C++ Multithreaded Cache Engine

A Redis-inspired in-memory cache engine written in C++17. The repository currently contains a thread-safe storage layer, a thin cache orchestration wrapper, a simple command parser, a Windows TCP server skeleton, and a concurrency test harness.

## Current Status

This project is in active development. The codebase already includes:

- a thread-safe `StorageEngine` built on `std::unordered_map` and `std::shared_mutex`
- a `CacheEngine` facade over the storage layer
- a command parser for local CLI-style commands
- a WinSock TCP server skeleton that binds, listens, and accepts a client connection
- a multithreaded test that exercises concurrent reads and writes against the storage layer

The longer-term design documented in `plan.md` and `outputes/` covers sharding, TTL, eviction, persistence, replication, and metrics.

## Features In The Current Codebase

- Thread-safe key/value storage
- `SET`, `GET`, `DELETE`, `ALL`, and `EXIT` command parsing
- Windows socket setup through WinSock
- A simple concurrency test for `StorageEngine`

## Repository Layout

```text
.
├── CMakeLists.txt
├── README.md
├── info.txt
├── plan.md
├── outputes/
│   ├── beginners_guide.md
│   ├── overview.md
│   └── system_design.md
├── src/
│   ├── main.cpp
│   ├── cache/
│   │   ├── CacheEngine.cpp
│   │   └── CacheEngine.h
│   ├── command/
│   │   ├── Command.h
│   │   ├── CommandParser.cpp
│   │   └── CommandParser.h
│   ├── network/
│   │   ├── TcpServer.cpp
│   │   └── TcpServer.h
│   └── storage/
│       ├── Entry.h
│       ├── StorageEngine.cpp
│       └── StorageEngine.h
└── tests/
	└── ThreadTest.cpp
```

## Build Requirements

- CMake 3.10 or newer
- A C++17-capable compiler
- Windows with WinSock libraries available for the network target

## Build

From the repository root:

```powershell
cmake -S . -B build
cmake --build build
```

## Run

The main executable currently starts the TCP server on port `8080`.

```powershell
.\build\cpp_multithreaded_cache_engine.exe
```

## Tests

The test target exercises the storage layer from multiple threads.

```powershell
.\build\cpp_multithreaded_cache_engine_tests.exe
```

## Command Parser

The current parser accepts the following commands:

- `SET <key> <value...>`
- `GET <key>`
- `DELETE <key>`
- `ALL`
- `EXIT`

`SET` supports multi-word values, and unknown or malformed commands are reported as invalid.

## Implementation Notes

- `StorageEngine` uses a `std::shared_mutex` so multiple readers can proceed in parallel while writes remain exclusive.
- `CacheEngine` is currently a thin wrapper around `StorageEngine`, which keeps the cache policy and storage mechanics separated.
- `TcpServer` currently performs the socket lifecycle setup and accept loop entry, but command execution over the network is still a work in progress.

## Roadmap

The design notes in `plan.md` and `outputes/system_design.md` describe the planned next steps:

- sharded cache storage
- TTL support
- eviction policy
- persistence via snapshot and WAL-style logging
- replication
- metrics and observability

