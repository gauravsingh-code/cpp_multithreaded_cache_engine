Status: User requested implementation kickoff on 2026-04-26. Plan approved for handoff to implementation agent.


## Plan: C++ Multithreaded Cache Engine Blueprint

Build a Redis-like, multithreaded in-memory cache engine in C++ with mixed burst workload handling, TTL, eviction, memory limits, metrics, persistence, replication, and pluggable serialization. Use a phased architecture: first a correct single-node core with strong concurrency design, then durability and replication, then operational hardening.

**Steps**
1. Phase 1: Requirements and contracts
1. Define API contract for Redis-like CLI commands for MVP (GET, SET, DEL, EXPIRE, TTL, MGET, MSET, INFO, SAVE) and expected error semantics.
2. Define non-functional SLOs: p50/p99 latency targets, throughput target under burst traffic, memory ceiling behavior, replication lag targets.
3. Finalize eviction policy strategy for mixed workload: start with LRU for predictable behavior, leave extension point for LFU.

1. Phase 2: Core architecture and data model
1. Partition keyspace into fixed shards (for example 64 or 128 shards) using consistent hash modulo to reduce lock contention.
2. Per shard, design data structures: hash table for entries, intrusive/access list for LRU, min-heap or timing wheel index for TTL, and per-shard memory accounting.
3. Define entry model: key bytes, value bytes/object, metadata (version, expiry timestamp, last-access tick, flags).
4. Design serialization abstraction with interface boundary so value encoding can be swapped without touching cache core.

1. Phase 3: Concurrency model
1. Use a boss-worker networking model with epoll/kqueue/select abstraction for CLI connections and a bounded worker thread pool for command execution.
2. Route each command to target shard(s); single-shard commands use shard mutex/shared_mutex, multi-key commands use deterministic lock ordering to avoid deadlock.
3. Add backpressure and admission control: bounded request queue, per-connection limits, overload response path for burst protection.
4. Implement background maintenance threads: expiration sweeper, eviction coordinator, snapshot writer, metrics aggregator.

1. Phase 4: Memory and lifecycle controls
1. Enforce max memory with high/low watermarks.
2. On memory pressure, trigger eviction loop by shard with fairness; eviction candidates selected via LRU (later optional LFU mode).
3. TTL expiration strategy: lazy on read + active periodic sweep to prevent stale buildup.
4. Add allocator/memory diagnostics hooks (allocation totals, fragmentation estimate, per-shard usage).

1. Phase 5: Durability
1. Implement snapshotting first (point-in-time state dump) with copy-on-write aware coordination to avoid long request stalls.
2. Add append-only log (AOF/WAL) for incremental durability; include replay ordering guarantees and checksum validation.
3. Recovery path: load latest snapshot, replay WAL tail, validate key version monotonicity.

1. Phase 6: Replication
1. Start with primary-replica asynchronous replication.
2. Define replication stream protocol: command log with sequence numbers, heartbeat, ACK window.
3. Implement initial full sync + incremental catch-up.
4. Expose replication health metrics (lag bytes/time, sync status, last applied sequence).

1. Phase 7: Observability and operability
1. Metrics endpoint and INFO-like command: hit ratio, ops/sec, latency histograms, eviction/expiration rates, queue depth, worker utilization, memory by shard.
2. Structured logging with correlation id per request.
3. Admin controls: runtime config for limits, thread counts, sweep intervals.

1. Phase 8: Validation and hardening
1. Unit tests: shard map correctness, TTL semantics, eviction ordering, serialization compatibility.
2. Concurrency tests: race detection, deadlock checks, lock-order enforcement, correctness under parallel writers/readers.
3. Fault tests: crash during snapshot/WAL flush, truncated WAL, replica disconnect/reconnect.
4. Performance tests: sustained and burst load profiles, tail latency under memory pressure, recovery time benchmarks.

**Relevant files**
- No existing files in workspace yet; project skeleton and module layout will be created during implementation.

**Verification**
1. Build and run unit/integration test suites on Linux and Windows.
2. Execute benchmark scenarios for mixed burst traffic and compare against target SLOs.
3. Run durability recovery drills (snapshot plus WAL restore) and validate data correctness.
4. Run replication drills (full sync, lag, fail/reconnect) and validate eventual consistency targets.
5. Run long soak test with memory cap and TTL churn; verify no unbounded growth and stable p99.

**Decisions**
- Implementation language: C++.
- Workload target: mixed traffic with bursts.
- Included capabilities: TTL, eviction, memory limit, metrics, persistence, replication, pluggable serialization.
- Access model: Redis-like CLI server interface.
- Recommended sequencing: single-node correctness and performance first, then persistence, then replication.

**Further Considerations**
1. Network protocol scope for v1: strict RESP compatibility versus RESP-like custom subset. Recommendation: start with RESP-like subset for faster delivery, then add strict compatibility.
2. Eviction policy choice for v1: LRU only versus runtime switch LRU/LFU. Recommendation: implement LRU first with a policy interface, add LFU after baseline stabilization.
3. Durability default mode: snapshot-only versus snapshot plus WAL. Recommendation: ship snapshot first for simplicity, then enable WAL for stronger recovery point objective.