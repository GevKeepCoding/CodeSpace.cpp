
## two important things:
* Latency
* Bandwidth



![bandwidth](images/bandwidth.png)


write-only will be slower than read-only and same as read-write.
this is because Write-only needs to update cache(likely L3), requesting a read and doubling the RAM-CPU
traffic.

---
The solution is bypassing caches.
look at the code for more details.

---
## 4. Latency
To understand latency we need pointer chasing.
The results:
* after you'll have more memory than L3 cache(4M in our example) the perf will be much worse.
* If you will make cpu frequency go up from 2 to 4GHz the perf will ~almost double when data is
	small enough to be in L3 cache and very small speedup when it's more than L3.

## 5. Cache Lines
* The cache line is 64 bytes.
```C++
void test1(int D = 1) {
	for (int i = 0; i < N; i += D)
		a[i]++;
	}
```
D = 16 needs 1 scalar instruction
2x faster if data is in L1.
when data is bigger than L2, the perf will not be visible.

D = 1  needs 2 vector instructions

**Tip: measure memory in terms of cache lines**
**Tip: use memory alignment to minimize cache line fetches**

---
**lstopo on linux will show the numa nodes and memory**
### Option 1:
** parallel taskset -c 0,1,2,3 ./app ::: {0..3}**
will run 4 processes on first 4 cores

### Option 2:
**numactl --interleave=all ./app**
	
•	--cpunodebind=<nodes>: This option binds the process to the CPUs of the specified node(s).
	The <nodes> parameter is a comma-separated list of node numbers.
•	--membind=<nodes>: This option binds the process to the memory of the specified node(s).
	The <nodes> parameter is a comma-separated list of node numbers.
•	--preferred=<node>: This option sets the preferred node for memory allocation.
	The process will allocate memory from this node as long as there is free memory available.
•	--localalloc: This option sets the local allocation policy, which means the process
	will allocate memory on the node where it is currently executing.
•	--physcpubind=<cpus>: This option binds the process to the specified CPUs.
	The <cpus> parameter is a comma-separated list of CPU numbers.
•	--show: This option shows the NUMA policy settings of the current process.
•	--hardware: This option shows the available NUMA nodes and their associated CPUs
	and memory.

---
