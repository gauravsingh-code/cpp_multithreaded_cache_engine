# Engineering Directive: Phase 1 — Core Cache Engine & Concurrency Design

Welcome to Phase 1 of your C++ Cache Engine. As your tutor and manager, I expect you to build this core with clean, production-grade C++17. The engine's foundation must be rock-solid before we introduce networking or durability.

This directive details **what** you need to build, **why** each component is necessary, and the **engineering principles** you must follow.

---

## 1. Directory Blueprint
Create the following structure in your workspace:
```
cpp_multithreaded_cache_engine/
├── CMakeLists.txt
├── include/
│   └── cache/
│       ├── entry.hpp
│       ├── eviction_policy.hpp
│       ├── shard.hpp
│       └── cache_engine.hpp
├── src/
│   ├── cache/
│   │   ├── eviction_policy.cpp
│   │   ├── shard.cpp
│   │   └── cache_engine.cpp
│   └── main.cpp
└── tests/
    └── unit/
        └── test_cache_core.cpp
```

---

## 2. Component Design & Rationale

### A. CMake Configuration (`CMakeLists.txt`)
- **What to do**: Set up a C++17 CMake project compiling a static library (`cache_engine_lib`) for the core cache engine source files, linking with the system thread library (`Threads::Threads`). Define target executables for `main.cpp` and `test_cache_core.cpp`.
- **Why this is necessary**: Modern CMake ensures cross-platform building across Windows (MSVC) and Unix-like systems. Forcing warnings as errors (`/WX` on MSVC, `-Werror` on GCC/Clang) guarantees you write clean, warning-free code from day one.

### B. Cache Entry Metadata (`include/cache/entry.hpp`)
- **What to do**: Design a `CacheEntry` struct containing:
  - `std::string value` (stored payload).
  - `uint64_t expiration_time` (absolute millisecond epoch, `0` for persistent).
  - `uint64_t last_accessed` (absolute epoch updated on read/write).
  - `uint64_t version` (monotonically increasing write version count).
  - `uint32_t flags` (custom encoding/serialization flags).
  - An estimate memory usage method: `size_t estimate_memory_usage(const std::string& key) const` summing capacity of dynamic objects and the struct size.
- **Why this is necessary**: Memory capacity controls and eviction decisions require strict size-tracking. Using capacity (`capacity()`) rather than size (`size()`) of strings reflects actual heap consumption. Version numbers prevent split-brain issues during replication later on.

### C. Eviction Policy Interface & LRU (`include/cache/eviction_policy.hpp`, `src/cache/eviction_policy.cpp`)
- **What to do**: Design an abstract `EvictionPolicy` class with virtual methods for recording key access, recording key removal, and selecting/popping an eviction candidate. Create a derived `LruEvictionPolicy` using a doubly-linked list (`std::list<std::string>`) to track access ordering, and a hash map (`std::unordered_map<std::string, std::list<std::string>::iterator>`) for O(1) node tracking.
- **Why this is necessary**: Standard library list erasure is $O(N)$ unless you keep iterators. Mapping keys to list iterators enables $O(1)$ updates on hit. Do not write locks inside the eviction policy; the Shard container will manage concurrency externally to avoid double-locking overhead.

### D. Concurrency Shard Layer (`include/cache/shard.hpp`, `src/cache/shard.cpp`)
- **What to do**: Design a `Shard` class managing a subset of keys. It must encapsulate:
  - `std::shared_mutex` for reader-writer locking.
  - `std::unordered_map<std::string, CacheEntry>` for key-value store.
  - `std::multimap<uint64_t, std::string> ttl_index` mapping absolute expiration time to keys.
  - `LruEvictionPolicy` for eviction tracking.
- **Why this is necessary**: A single global lock on the entire cache creates extreme lock contention under high concurrent requests. By sharding keyspace, concurrent operations access different shards, yielding parallel speedup.
- **Locking Rules**:
  - Reading (`get`): Acquire `std::shared_lock`.
  - Mutating (`set`, `del`, `expire`, `evict`): Acquire `std::unique_lock`.
  - Lazy Expiration: When reading a key, if it is expired, upgrade the lock/perform write operations to purge it, and return "not found".

### E. Orchestration Engine & Active TTL Sweeper (`include/cache/cache_engine.hpp`, `src/cache/cache_engine.cpp`)
- **What to do**: Build `CacheEngine` managing an array of `Shard` instances (default to 64). Compute routing via hashing: `hash_fn(key) % shard_count`.
- **Memory Tracking**: Maintain an atomic counter (`std::atomic<size_t>`) for the total memory used. Every operation returns `memory_delta` which updates this counter.
- **Active TTL Sweeper Thread**: Spawn a background thread that wakes up periodically (e.g. 100ms), picks a batch of random shards, locks them, and cleans up expired keys matching the `ttl_index` to prevent memory leaks from inactive/un-accessed expired keys.
- **Inline Eviction**: If `set()` causes total memory to exceed `max_memory_limit`, perform inline LRU evictions on the active shard until the memory drops below the threshold.

---

## 3. Managerial Guidance (Code Review Guidelines)

1. **Deadlock Prevention**: When writing tests or utility commands accessing multiple keys across shards, always lock shards in a deterministic order (e.g., sorting shard IDs) to prevent deadlock states.
2. **Lazy + Active TTL Sweeping**: Lazy sweeping cleans hot keys, while active periodic sweeping cleans cold keys. Both are mandatory.
3. **No Raw Pointers**: Use C++ smart pointers (`std::unique_ptr`, `std::shared_ptr`) or value semantics to avoid memory leaks.

*Let me know when you have set up this directory and need the detailed algorithmic walkthrough for the Shard class implementation.*
