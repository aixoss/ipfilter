# IPFilter — Active Optimisations Reference

This document is the **single authoritative list** of all optimisations currently
active in the codebase.  It reflects a cross-check of all source files against the
`Makefile` as of the current shipped state (`git diff e8083ee HEAD`).

For full design rationale see [`IPFILTER_PERF_CHANGES.md`](IPFILTER_PERF_CHANGES.md).
For build recipes see [`BUILD.md`](../../../BUILD.md).
For bug history see [`BUGS_IDENTIFIED_AND_FIXED.md`](BUGS_IDENTIFIED_AND_FIXED.md).

---

## Background

AIX IPFilter (`ipfl`) on a high-CPU-count (e.g. 64-way POWER11) system originally
suffered a **>95% TPS regression** even with a pass-all ruleset, caused by
**reader-count cacheline contention** on five single shared `DRW_lock` globals.
The optimisation programme replaced those locks with per-CPU arrays and eliminated
a series of further hotspots identified through iterative `tprof` profiling.

All optimisations are **unconditional code** — no `#ifdef` guards, no compile-time
flags.  The optimised path is the only path in the source.  The only build-time
knob remaining is `IPSTATE_SIZE=16384` set in the `Makefile`.

---

## Current TPS Baseline (Final Measured — POWER11 16-core, 100GbE, TCP_RR 800 sessions, 1-byte)

| Configuration | TPS (server) | CPU% (server) | Avg Resp (ms) | vs No-IPF | Notes |
|---|---|---|---|---|---|
| No IPFilter loaded | **3,072,240** | 76.25% | 0.26 ms | — | |
| Original IPFilter (unoptimised) | **125,129** | 60.12% | 6.39 ms | **−95.9%** | |
| **Shipped IPFilter (all active opts)** | **2,601,175** | 67.12% | 0.31 ms | **−15.3%** | Current shipped state |

**Key metrics:**

| Metric | Value |
|---|---|
| TPS recovered vs original | +2,476,046 TPS (+1,979%) |
| Gap closed (original → no-IPF) | **84.0%** of the original performance gap |
| Response time improvement | 6.39 ms → 0.31 ms (**20.6× better**) |
| Remaining degradation vs no-IPF | −15.3% (471,065 TPS gap) |

---

## Category 1 — Permanently Applied, Unconditional

These optimisations have no `#if` / `#ifdef` guards.  The optimised path is the
only path in the source.

---

### Per-CPU DRW Lock Infrastructure (Parts A + Opt 21)

**Root problem:** The original code used a single AIX `DRW_lock` per subsystem.
Every `drw_lock_read()` performs a CAS increment on a shared reader-count
cacheline; with 20+ CPUs processing packets simultaneously this caused continuous
cross-CPU cacheline bouncing.

**Fix:** Each of the five hot-path global locks was replaced with an array of
256 per-CPU DRW locks (`ipfrwlock_percpu_t`).  Readers lock only their own CPU
slot — zero cross-CPU traffic.  Writers sequentially lock all 256 slots.

| Lock converted | Subsystem | Per-CPU replacement |
|---|---|---|
| `ipf_mutex` / `ipf_global` | Core filter rules | `ipf_global_pcpu` |
| `ipf_frcache` | Rule cache | `ipf_frcache_pcpu` |
| `ipf_state` | Connection state table | `ipf_state_pcpu` |
| `ipf_nat` | NAT table | `ipf_nat_pcpu` |
| `ipf_frag` | Fragment cache | `ipf_frag_pcpu` |
| `ip_poolrw` | IP pool / hash table | `ip_pool_pcpu` (Opt 21) |

The old `ip_poolrw` global is gone from `ip_pool.c`; `ip_fil.h` defines
`#define ip_poolrw ip_pool_pcpu` for any remaining reference sites.  The six
per-CPU lock arrays are declared in `ip_fil_aix.c` and initialised by
`RWLOCK_INIT_PERCPU` in `iplattach()`; torn down by `RW_DESTROY_PERCPU` in
`ipldetach()`.

Three new fields were added to `fr_info_t` (in [`netinet/ip_fil.h`](netinet/ip_fil.h))
to carry per-CPU lock-slot indices across function boundaries:

| Field | Purpose |
|---|---|
| `fin_cpuidx` | `ipf_state_pcpu` slot; set by `fr_stlookup()`, released by `fr_checkstate()` |
| `fin_nat_cpuidx` | `ipf_nat_pcpu` slot; threaded through proxy callbacks |
| `fin_global_cpuidx` | `ipf_global_pcpu` slot captured at `fr_check()` entry; passed to `PERCPU_STATS_INCL` |

All three are initialised to `IPF_PERCPU_IDX_INIT` (-1) before use.

**Files:** [`netinet/ip_compat.h`](netinet/ip_compat.h),
[`netinet/ip_fil.h`](netinet/ip_fil.h),
[`ip_fil_aix.c`](ip_fil_aix.c),
[`fil.c`](fil.c),
[`ip_state.c`](ip_state.c),
[`ip_nat.c`](ip_nat.c),
[`ip_frag.c`](ip_frag.c),
[`ip_pool.c`](ip_pool.c),
[`ip_htable.c`](ip_htable.c),
[`ip_lookup.c`](ip_lookup.c)

---

### Opt 12 — Consolidate `ipf_mutex_pcpu` into `ipf_global_pcpu`

**Problem:** The original code acquired both `ipf_mutex_pcpu` (core rules) and
`ipf_global_pcpu` (lifecycle guard) separately on every packet — two full DRW
lock cycles per packet.

**Fix:** All `ipf_mutex_pcpu` acquire/release sites were replaced with
`ipf_global_pcpu`.  The `READ_ENTER(&ipf_mutex)` before `fr_checkauth()` and the
`RWLOCK_EXIT(&ipf_mutex)` calls on the `goto finished` paths in `fr_check()` are
all gone.  One lock, one DRW cycle per packet.

**Measured gain:** +~19,000 TPS.

**Files:** [`fil.c`](fil.c), [`ip_fil_aix.c`](ip_fil_aix.c)

---

### Opt 1 — Per-CPU `frstats_pcpu` Counters

**Problem:** `ATOMIC_INCL(frstats[...])` was called ~28 times per packet.  Each
call issued a lock-prefix CAS (`fetch_and_addll`) on the globally-shared
`frstats[]` array, causing cross-CPU cacheline contention on the stats counters.

**Fix:** One `filterstats_t` slot per CPU (`frstats_pcpu[IPF_MAX_CPUS][2]`,
cacheline-aligned at 128 bytes).  Each CPU increments only its own slot with the
non-atomic `PERCPU_STATS_INCL(fin->fin_global_cpuidx, out, field)` macro.
`fr_getstat()` aggregates across all slots at read time; `fr_zerostats()` zeroes
all slots.  The original `struct filterstats frstats[2]` global is gone.

**Files:** [`fil.c`](fil.c:178), [`netinet/ip_fil.h`](netinet/ip_fil.h),
[`ip_fil_aix.c`](ip_fil_aix.c), [`ip_proxy.c`](ip_proxy.c)

---

### Opt 3 — No-Rules / No-State Fast Path

**Problem:** Even with a completely unconfigured filter (no rules, no state, no
NAT, no auth, no fragment tracking), every packet still traversed the full
`fr_check()` setup path.

**Fix:** Immediately after acquiring `ipf_global_pcpu` and confirming
`fr_running > 0`, check all subsystem enable flags under the read lock.  If all
are inactive, release the lock and return the default policy immediately.

**Files:** [`fil.c`](fil.c:2383)

---

### Opt 4 — Cacheline-Align Per-CPU Data Structures

**Problem:** Adjacent CPU slots in `frstats_pcpu[]` and the per-CPU lock arrays
shared cachelines, causing false sharing: a write to CPU 0's slot would
invalidate CPU 1's slot on a different core.

**Fix:** `__attribute__((aligned(128)))` on `frstats_pcpu[IPF_MAX_CPUS][2]` and
all six `ipfrwlock_percpu_t` lock arrays.  128 bytes covers two POWER cachelines.

**Files:** [`fil.c`](fil.c:178), [`ip_fil_aix.c`](ip_fil_aix.c)

---

### Opt 12B — Remove `fr_ref` Bump + `fr_derefrule` in `fr_check()`

**Problem:** The original `fr_check()` bumped `fr->fr_ref` under `MUTEX_ENTER`
before releasing `ipf_mutex`, then called `fr_derefrule(&fr)` after the
`fr_fastroute`/ICMP block — a mutex acquire + release per packet just to keep
`fr` alive for ~10 instructions.

**Fix:** `ipf_global_pcpu` is held in read mode for the entire body of
`fr_check()` including the fastroute/ICMP block, so no writer can free `fr`
during that window.  The `fr_ref` bump, `MUTEX_ENTER`, `MUTEX_EXIT`, and the
trailing `fr_derefrule` call are all removed.

**Files:** [`fil.c`](fil.c:2591)

---

### Opt 19A — Sub-Lock Elision: NAT Initial Acquire (partial)

**Status:** Opt 19A was originally specified to elide read locks from six
sub-functions.  After the security audit, the `ipf_frcache_pcpu` and
`ipf_state_pcpu` read-path sites were deliberately **restored**.  Only the NAT
initial-acquire elision survives.

| Function | Lock elided? | Current state |
|---|---|---|
| `fr_firewall()` | `ipf_frcache_pcpu` | **Locks restored** — `READ_ENTER_PERCPU` / `RWLOCK_EXIT_PERCPU` present at `fil.c:2164/2170/2178` |
| `fr_knownfrag()` | `ipf_frag_pcpu` | **Still elided** — no read acquire; comment preserved at `ip_frag.c:638` |
| `fr_stlookup()` | `ipf_state_pcpu` | **Read lock restored** at function entry (`ip_state.c:2349`); see Per-CPU State Lock section below |
| `fr_checkicmpmatchingstate()` | `ipf_state_pcpu` | **Locks restored** at `ip_state.c:2153/2209` |
| `fr_checknatout()` | `ipf_nat_pcpu` | **Still elided** — initial acquire skipped; only write-upgrade path takes the lock |
| `fr_checknatin()` | `ipf_nat_pcpu` | **Still elided** — same pattern as `fr_checknatout()` |

The NAT elision pattern: `ipf_cpu_idx_nat` is initialised to `IPF_PERCPU_IDX_INIT`
and stored in `fin->fin_nat_cpuidx` without acquiring the lock.  On a cache hit
(existing NAT entry) the function returns without ever taking `ipf_nat_pcpu` read
mode.  Only the write-upgrade path (new entry creation) calls
`RWLOCK_EXIT_PERCPU` (no-op on `IPF_PERCPU_IDX_INIT`) then
`WRITE_ENTER_PERCPU`.

**Do not re-elide** the `ipf_state_pcpu` and `ipf_frcache_pcpu` sites without
completing the security audit review — those locks were restored deliberately.

**Estimated gain retained:** ~0.3–0.4% CPU (NAT + frag elision only; the dominant
`ipf_state_pcpu` read-path gain is gone).

**Files:** [`ip_nat.c`](ip_nat.c:3715), [`ip_frag.c`](ip_frag.c:638)

---

### Per-CPU State Lock with `fin_cpuidx` Threading

**Problem (security fix F1/F2):** The original Opt 19A fully elided
`ipf_state_pcpu` from `fr_stlookup()`.  The security audit identified this as
unsafe: `ipf_global_pcpu` is a lifecycle guard only — it does not protect
`ips_table[]` from concurrent `fr_addstate()` / `fr_delstate()` which hold only
`ipf_state_pcpu` write.  Walking `is->is_hnext` while `fr_delstate()` frees the
same entry is a use-after-free.

**Fix (F1 + F2):** `fr_stlookup()` acquires `READ_ENTER_PERCPU(&ipf_state_pcpu,
fin->fin_cpuidx)` at function entry before any hash chain traversal.  The slot
index is stored in `fin->fin_cpuidx` (new field in `fr_info_t`, placed beyond
`FI_COPYSIZE` so cache-hit `bcopy` never overwrites it).  `fr_checkstate()`
releases the slot after bumping `is_ref`.  The write-upgrade paths
(`retry_tcpudp`, `icmp6again`) release the read slot before calling
`WRITE_ENTER_PERCPU` to avoid self-deadlock.

This also replaced all `MUTEX_DOWNGRADE(&ipf_state)` calls with
`MUTEX_DOWNGRADE_PERCPU(&ipf_state_pcpu, fin->fin_cpuidx)`.

**Files:** [`ip_state.c`](ip_state.c:2339), [`netinet/ip_fil.h`](netinet/ip_fil.h)

---

### Opt 25 — `/32` Exact-Match Fastpath in `ip_pool_search()`

**Problem:** `ip_pool_search()` always ran the full radix traversal
(`rnh_matchaddr`), even for pools containing a single `/32` host entry where the
result degenerates to a single 32-bit comparison.

**Fix:** At the top of `ip_pool_search()`, check `ipo_list->ipn_next == NULL` and
`ipn_mask == 0xffffffff` before entering the radix path.  On match, perform a
single `addr->in4.s_addr == only->ipn_addr.adf_addr.in4.s_addr` compare.
Falls through to the full radix path for multi-entry or non-`/32` pools.

**Estimated gain:** ~0.4–0.7% CPU in pool-heavy workloads.

**Files:** [`ip_pool.c`](ip_pool.c:435)

---

### Opt 28 — Elide `ip_pool_pcpu` Lock from Opt 25 Fastpath

**Problem:** The Opt 25 fastpath still acquired `ip_pool_pcpu` on every hit,
paying `mycpu()` (~15 cycles) + 6 slot-index instructions + `drw_lock_read` +
`drw_lock_done`.

**Fix:** The lock is not acquired in the fastpath.  The design invariant:
the entire fastpath is covered by `ipf_global_pcpu` READ lock.  The old
`READ_ENTER(&ip_poolrw)` / `RWLOCK_EXIT(&ip_poolrw)` pair is removed from
the radix-walk path as well — `ip_pool_pcpu` (the per-CPU replacement) is only
taken by ioctl write paths.

**Files:** [`ip_pool.c`](ip_pool.c:439)

---

### OPT-NEW-B — Empty-Pool Early Exit in `ip_pool_search()`

**Problem:** If a pool is flushed (`ippool -F`) without a subsequent reload,
`ipo->ipo_list == NULL`.  The radix walk would still execute and traverse to a
leaf with no match.

**Fix:** `__builtin_expect(ipo->ipo_list == NULL, 0)` guard before the Opt 25
fastpath and radix walk.  Returns `-1` immediately for empty pools.

**Files:** [`ip_pool.c`](ip_pool.c:424)

---

### Security Fix — `ip_pool_flush()` Partial-Flush Correction

**Problem:** `ip_pool_flush()` used `break` on the first `ip_pool_destroy()` error
(e.g. `EBUSY` when a rule still holds a reference), silently leaving all
subsequent pools un-flushed.  `ippool -f` would then hit `EEXIST` trying to
re-create those pools.

**Fix:** Replace `break` with `err_count++`; continue iterating all pools.  Return
`-err_count` if any errors occurred (negative value signals partial flush to
`ip_lookup.c`), or `num` (pools destroyed) on full success.  `ip_lookup.c` now
checks for a negative return and maps it to `EBUSY`.

**Files:** [`ip_pool.c`](ip_pool.c:695), [`ip_lookup.c`](ip_lookup.c)

---

### Opt 36 — Atomic Refcount in `fr_statederef` / `fr_checkstate`

**Problem:** `fr_statederef()` unconditionally acquired `WRITE_ENTER_PERCPU` on
every call, even when the state entry was still referenced by other threads
(`is_ref > 1`).  `fr_checkstate()` used `MUTEX_ENTER(&is->is_lock)` to bump
`is_ref` before releasing the state lock.

**Fix:**
- `fr_statederef()`: `ATOMIC_DEC32_RET(is->is_ref)` — if the old value was > 1,
  return immediately without taking any lock.  Only take `WRITE_ENTER_PERCPU` on
  the rare last-reference path (`old == 1`) to free the entry.
- `fr_checkstate()`: `ATOMIC_INC32(is->is_ref)` — bump the refcount atomically
  while still holding the `ipf_state_pcpu` read lock, removing the need for
  `is_lock`.

**Files:** [`ip_state.c`](ip_state.c:3979), [`ip_state.c`](ip_state.c:2753)

---

### Opt 37B — Skip `fr_movequeue` for Steady-State TCP ACKs

**Problem:** `fr_updatestate()` called `MUTEX_ENTER(&is->is_lock)` and
`fr_movequeue()` on every state hit.  For established TCP the `ifq` argument from
`fr_checkstate` is always `NULL` and `TQE_RULEBASED` is never set, so both the
lock and the queue move are unconditional no-ops on the dominant path.  `fr_tcp_age()`
similarly called `fr_movequeue` on every TCP packet even when the FSM state and
target queue were unchanged.

**Profile evidence (`no_opt37.prof`, 800-session TCP_RR, 820,631 TPS):**

| Function | % CPU | Root cause |
|---|---|---|
| `.slock` (spin loop) | **40.65%** | Contention on `ips_tqtqb[ESTABLISHED]->ifq_lock` |
| `.disable_lock` | **6.67%** | `MUTEX_ENTER` preamble at every `fr_movequeue` call |
| **Total `ifq_lock` cost** | **~47%** | Every ACK on every session serialises on one queue mutex |

TPS with Opt 37 reverted: **820,631** vs **2,601,175** with Opt 37 — a **−68% regression**.

**Root cause of the original Opt 37 crash:** `fr_tcp_age()` reached
`fr_movequeue(tqe, tqe->tqe_ifq, …)` when `tqe->tqe_ifq == NULL`.  Race window:
`fr_addstate()` inserts the new entry into the state hash table under write lock,
then releases the write lock and calls `fr_setstatequeue()`.  A packet matching
the entry in the window between hash insertion and `fr_setstatequeue()` called
`fr_tcp_age()` with `tqe_ifq == NULL`, causing a DSI PROC trap.

**Fix (Opt 37B):** In `fr_tcp_age()`, three-way branch replacing the unconditional
`fr_movequeue` call:
1. `tqe_ifq == NULL` — entry not yet queued; skip (safe).
2. FSM state changed or entry is on the wrong queue — call `fr_movequeue` as before.
3. State and queue unchanged (dominant steady-state ACK path) — refresh expiry deadline only.

In `fr_updatestate()`, skip the entire `MUTEX_ENTER` / `fr_movequeue` / `MUTEX_EXIT`
block when `ifq == NULL` and `TQE_RULEBASED` is clear (which is always true on the
established TCP path).

Additionally, `is_pkts[]` and `is_bytes[]` are now incremented with
`ATOMIC_INC64` / `ATOMIC_ADD64` (atomic fetch-and-add) outside the lock, rather
than under `MUTEX_ENTER(&is->is_lock)`.  This is safe because the state lock is
not needed for these counters — they use `fetch_and_addlp` which is inherently
atomic on POWER.

**Files:** [`ip_state.c`](ip_state.c:3564), [`ip_state.c`](ip_state.c:2594)

---

## Category 2 — Always-On via Makefile

These are set unconditionally in [`Makefile`](Makefile) per-file `_CFLAGS` and
apply to every build.

| Makefile setting | Effect |
|---|---|
| `ip_state.o_CFLAGS` / `ip_state.64o_CFLAGS`: `-DIPSTATE_SIZE=16384` | Increases the state hash table from 127 slots to 16,384.  Reduces average chain length from ~6.3 entries to ~0–1, eliminating most `fr_matchsrcdst` loop iterations. |

**Note:** There are no other compile-time optimisation flags in the Makefile.  All
other optimisations listed in Category 1 are baked unconditionally into the source.

---

## Optimisations Retired or Backed Out (do not retry)

| Opt | Flag | Reason |
|---|---|---|
| **Opt 19A (full elision)** | — | **🟡 Partially reverted by security audit (F1/F2).** `ipf_frcache_pcpu` and `ipf_state_pcpu` read locks restored in `fr_firewall()`, `fr_stlookup()`, and `fr_checkicmpmatchingstate()`. Only NAT initial-acquire and `fr_knownfrag()` elision survive. Do not re-elide restored sites until security audit is complete. ~1.2–1.5% CPU lost vs full elision. |
| **Bug 3 Fix (Option C — Read-Locked Re-probe)** | — | Described in earlier analysis but **never landed in `src`**. The `src` tree goes directly `RWLOCK_EXIT_PERCPU` → `WRITE_ENTER_PERCPU` on the `iss_wild` miss path without any read-lock re-probe intermediate step. Exists only in `src.perf5/6` branches. |
| **Opt 17** | `_INTR` macros | **🔴 Critical crash (Bug 6):** `RWLOCK_EXIT_PERCPU_INTR` silently mishandles `IPF_WRITE_SENTINEL=255` — leaves 254 lock slots permanently write-locked → kernel trap and system hang. All `_INTR` sites replaced with standard `READ_ENTER_PERCPU` / `RWLOCK_EXIT_PERCPU`. |
| **Opt 37 (original)** | — | **🔴 Critical crash:** `fr_tcp_age()` called `fr_movequeue(tqe, tqe->tqe_ifq, …)` without a `tqe_ifq != NULL` guard.  Race between `fr_addstate` hash insert and `fr_setstatequeue()` → NULL deref DSI PROC trap.  **Fixed in Opt 37B (active above).** |
| **Opt 33** | `IPF_OPT33` | **🔴 Safety risk:** `DOUBLE_HASH` modulo replaced with bitwise AND.  `fr_statesize` is runtime-configurable and can be non-power-of-2 → OOB `ips_seed[]` access → kernel panic. No measurable gain justified this risk. Reverted. |
| **OPT-A** | `IPF_CACHE_PREFILTER_FIELDS` | Shadow-copied `fr_proto_cache`/`fr_mproto_cache` bytes into `frentry_t` to avoid `fru_ipf` pointer dereference in `fr_scanlist` prefilter.  Stalls at `fr_scanlist+0x1D4/+0x1DC/+0x1E8` (8.73% combined) were expected to be recoverable.  **Null result:** all three stalls were running in the shadow of the `fr_next` DRAM miss (`+0x11C`, 3.46%); removing them did not shorten the critical path because `fr_next` was already the bottleneck.  See `OPT_AB_NULL_RESULT_ANALYSIS.md`. |
| **OPT-B** | `IPF_CACHE_LINE_LAYOUT` | Full `frentry_t` field reorder: moved `fr_v`, `fr_type`, `fr_flags`, `fr_arg` into cache line 0; isolated `fr_lock` in its own cache line (104-byte padding).  **Null result:** `fr_v` miss (0.67%) was also hidden behind the `fr_next` miss; moving it to line 0 did not reduce total iteration time.  Reverted together with OPT-A.  See `OPT_AB_NULL_RESULT_ANALYSIS.md`. |
| **OPT-RULES-A** | `IPF_RULES_ARENA` | Contiguous arena allocation for `frentry_t` (root-group sentinels, `KMALLOCS` pre-alloc, `frflushlist`/`frrequest` arena paths).  Profiled across 3 runs (900-rule workload): Run 1 baseline 1,590,266 TPS; Run 2 +0.8% (noise); Run 3 +6.5% — within run-to-run variance at this level (~6%).  Rule-reload contamination masked any clean packet-processing gain.  **No demonstrated improvement in steady-state TPS.**  Code, struct fields (`fg_arena`, `fg_arena_used`, `fr_arena_group`), and all arena paths fully reverted. |
| **OPT-RULES-B** | `IPF_FRIPF_INLINE` | Embed `fripf_t` by value in `frentry_t.fr_dun`.  Stall at `fr_ipfcheck+0x2C` reduced 6.51%→5.88% in isolation, but with scattered allocation the DRAM miss from non-contiguous `frentry_t` dominates — no net TPS gain.  Reverted. |
| **OPT-RULES-C** | `IPF_RULES_PREFETCH` | `dcbt` prefetch in `fr_scanlist` inner loop.  1-slot worsened across Runs 1–3; 2-slot implemented but not profiled standalone.  Effective only with contiguous arena — depends on OPT-RULES-A.  Reverted together with OPT-RULES-A/B. |
| Opt 9 | `IPF_HASH_STATE_MIX` | 64-bit multiplicative XOR state-table hash.  No measurable gain; additive hash restored. |
| Opt 22 | `IPF_HASH_HTABLE_FAST` | Power-of-2 modulo → AND in `fr_iphmfind` — no `type=hash` pools in production workload; zero gain. |
| Opt 23 | `IPF_PREDICT_IPFCHECK` | `__builtin_expect` hints in `fr_ipfcheck` — XLC already makes correct branch layout; zero measurable gain. |
| Opt 24 | `IPF_INLINE_IPFCHECK` | `__attribute__((always_inline))` on `fr_ipfcheck` — no measurable gain at current function size. |
| Opt 26 | `IPF_MEMO_POOL_CACHE` | Per-packet 2-slot pool-lookup memoization cache — net regression (`fr_ipfcheck` 1.53% → 2.01%); combined with Opt 27 also zero net gain. |
| Opt 27 | `IPF_MEMO_POOL_DIRECT_MAP` | Direct-mapped cache probe — requires Opt 26; combined still no net gain. |
| OPT-NEW-A | `IPF_OPT_CACHE4` | 4-slot cache widening — requires Opts 26+27; no net gain. |
| Opt 31 | `IPF_INLINE_CACHE_MATCH` | Inline 5-field tuple compare replacing `bcmp` in `fr_firewall` — no measurable gain. |
| Opt 34 | `IPF_OPT34` | `always_inline` on `fr_matchsrcdst` — no measurable gain. |
| Opt 2 | — | Targeted `fr_info_t` init replacing full `bzero` — no measurable gain; `bzero((char *)fin, sizeof(*fin))` restored. |
| Opt 5 | — | `bcopy` → `__builtin_memcpy` for `FI_COPYSIZE` copy — no measurable gain; `bcopy` restored. |
| Opt 7 | — | `strncpy` → `__builtin_memcpy` for `fin_group` copies — no measurable gain; `strncpy` restored. |
| Opts 6A/B/C | — | Per-rule `fr_bytes` lazy accumulation — no gain; reverted. |
| Opt 15 | — | Comment out `fr_hits`/`fr_bytes` writes — cacheline contention hypothesis disproved. |
| Opt 18 | `IPF_PERCPU_LOCK_NOP` | NOP all percpu locks — locking is not the remaining bottleneck. |
| Opt 19 | `IPF_AUTH_LOCK_NOP` | NOP `fr_checkauth()` locks — call frequency too low; no measurable effect. |

---

## Pending Optimisations (not yet implemented)

| Opt | Flag | Description | Est. gain |
|---|---|---|---|
| **Opt 38** | None (unconditional) | Stripe `ips_tqtqb[IPF_TCP_NSTATES]` into `ips_tqtqb[IPF_TCP_NSTATES][IPF_TQ_NSTRIPES]` sub-queues.  Entry-to-stripe by `(uintptr_t)tqe >> 7 & MASK` — stable for entry lifetime.  Divides `ifq_lock` contention by `IPF_TQ_NSTRIPES` (16×) on remaining queue operations (state transitions, teardown, UDP/ICMP).  Design fully specified in `PENDING_OPTIMIZATIONS.md`. | ~contention/16 on remaining `ifq_lock` paths |
| **Opt 35** | `-DIPF_OPT35` | Per-CPU `ips_stats_pcpu[]` — replace `ATOMIC_INCL(ips_stats.iss_hits)` and all `iss_*` atomic increments with per-CPU non-atomic counters (`fetch_and_addll` at **4.40%** of all cycles in Profile 6). | +80K–120K TPS |
| **Opt 32** | None (unconditional) | Elide `ip_pool_pcpu` lock from full radix path in `ip_pool_search()` under `ipf_global_pcpu` — extends Opt 28 to all pool lookups, not just the `/32` fastpath. | +31K–40K TPS |
| **OPT-NEW-C** | Fold into pool flags | Replace `bzero(ipo_nodes, sizeof(ipo_nodes))` before `rnh_matchaddr` with a stack-declared zero-initialiser, letting XLC emit a single `dcbz`. | ~0.15–0.20% CPU |
| **OPT-17-SAFE** | `-DIPF_OPT17_SAFE` | Single `i_disable(PL_IMP)` at `fr_check()` entry shared by all nested sub-lock sites — safe re-implementation of retired Opt 17.  Requires full IPL audit. | ~+0.30–0.50% CPU |
| **OPT-NEW-E** | Investigation | Determine why `drw_lock_read_ppc` / `drw_lock_done_ppc` (POWER8 fallback DRW paths) appear at 0.59% on POWER11.  Unblock after next profile run. | ~18K TPS if fixed |
| **ACTION-2** | None | Move port-range check before pool src lookup inside `fr_ipfcheck`.  For 900-rule workload: 48.9M pool radix walks per run are wasted (99.9998% miss); a port check costs 1 cycle vs 50–100 ns per radix walk.  See `DEBUG_SESSION_MPROTO.md §12` and `PENDING_OPTIMIZATIONS.md`. | Structural (900-rule) |
| **ACTION-3** | Config change | Ensure `flags A/A` (pass quick, no state) rule appears first in every `fr_scanlist` group in `ipf.conf`.  Converts ~900 rule iterations per established-TCP packet to ~2.  Zero kernel rebuild. | Large (900-rule) |

---

## Related Documentation

| File | Contents |
|---|---|
| [`IPFILTER_PERF_CHANGES.md`](IPFILTER_PERF_CHANGES.md) | Full design description of every optimisation — infrastructure, algorithms, correctness proofs |
| [`BUGS_IDENTIFIED_AND_FIXED.md`](BUGS_IDENTIFIED_AND_FIXED.md) | All kernel bugs found during optimisation work, root-cause analysis, and fixes |
| [`PENDING_OPTIMIZATIONS.md`](PENDING_OPTIMIZATIONS.md) | Pending opts, profile results, and recommended implementation sequence |
| [`OPT_AB_NULL_RESULT_ANALYSIS.md`](OPT_AB_NULL_RESULT_ANALYSIS.md) | OPT-A/B post-mortem — why eliminating `fru_ipf` stalls yielded no TPS gain (fr_next DRAM miss is the floor) |
| [`PROFILE_OPT_PROTO_PREFILTER_3_ANALYSIS.md`](PROFILE_OPT_PROTO_PREFILTER_3_ANALYSIS.md) | tprof decode for the OPT-PROTO-PREFILTER_3 run — identified OPT-A/B candidates |
| [`DEBUG_SESSION_MPROTO.md`](DEBUG_SESSION_MPROTO.md) | `fr_mproto` root-cause investigation; ACTION-1/2/3 identification for 900-rule workload |
| [`PROFILE_900RULES_ANALYSIS.md`](PROFILE_900RULES_ANALYSIS.md) | 900-rule tprof decode — OPT-RULES-A/B/C investigation (all reverted) |
| [`PERIODIC_BURST_ANALYSIS.md`](PERIODIC_BURST_ANALYSIS.md) | Pre-opt 759 µs burst root cause and post-opt residual lock-hold analysis |
| [`PERF_OPTIMIZATION_FINDINGS.md`](PERF_OPTIMIZATION_FINDINGS.md) | Profile 1–4 analysis |
| [`PERF_OPTIMIZATION_FINDINGS_RULES.md`](PERF_OPTIMIZATION_FINDINGS_RULES.md) | Profile 5–6 analysis, pool-rule workload |
| [`BUILD.md`](../../../BUILD.md) | Canonical build reference — all build recipes |
