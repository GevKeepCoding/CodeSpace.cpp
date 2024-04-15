




# Transparent Huge Pages

* Does not require boot time configuration or special permissions
* Default page size is still 4k, but processes can madvise() use of large pages.
	* Allows specific apps to opt-in on demand
	* Benefits of smaller TLB with less wasted memory
# echo madvise > /sys/kernel/mm/transparent_hugepage/enabled
* Enable huge pages through use of madvise.
Example:
```
// use madvise to enable huge pages
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    // Allocate a large chunk of memory
    size_t size = 2 * 1024 * 1024; // 2MB
    void* addr = malloc(size);
    if (addr == NULL) {
        std::cerr << "Failed to allocate memory\n";
        return 1;
    }

    // Advise the kernel to use huge pages for this memory
    if (madvise(addr, size, MADV_HUGEPAGE) == -1) {
        std::cerr << "Failed to advise use of huge pages\n";
        free(addr);
        return 1;
    }

    std::cout << "Successfully advised use of huge pages\n";

    // Don't forget to free the memory when you're done
    free(addr);

    return 0;
}


```

# echo defer > /sys/kernel/mm/transparent_hugepage/defrag
* this command will defer instead of blocking large page request


# You can also pin memory/threads.
* Pinning memory or threads to a particular core can improve performance.
    * Reduces intra-core memory ownership transfers.
    * Less likely to have cache invalidations.

* isolcpu allows reservation of CPUs for non-kernel use with cpusets
```bash
#!/bin/bash

# Replace 'your_application' with the name of your application
your_application="your_application"

# Get the PID of the application
pid=$(pgrep -f $your_application)

# If the application is running, pin it to core 1
if [ ! -z "$pid" ]; then
    taskset -cp 1 $pid
else
    echo "Application is not running."
fi
```
* taskset allows binding of a process to a specific CPU or set of CPUs.
```bash
taskset -cp 1 1234
// bind process 1234 to core 1
// bind process 1234 to core 1 and 2
taskset -cp 1,2 1234

```
* numactl allows cores/memory to be clamped for a process
```bash
numactl --cpunodebind=0 --membind=0 ./your_application
// bind process to node 0
```
* Pining worker thread to core 2
```bash
taskset -cp 2 1234
```


#### On newer intel processors we have a way to do better recording with perf tool.
* To improve accuracy use :p, :pp or :ppp suffix to event.
* perf record -b program
* perf record --call-graph lbr -j any_call,any_ret program
* perf record - e intel_pt//u program

* perf stat base64 < (echo hello)    
Look at **insn per cycle** is a good metric to look at. >4 is a good number.  
perf -list to see options.
* perf stat -a --topdown sleep 1
* look at retiring if it is low, it means you are not using the CPU to its full potential.


#### Understanding CPU Microarchitecture to Increase Performance
https://www.youtube.com/watch?v=rglmJ6Xyj1c
dd if=/dev/urandom of=/tmp/rand bs = 4096 count=4096
./toplev.py --single-thread --no-multiplex -l1 -- base64 /tmp/rand > /dev/null


```c++
__builtin_expect(error,1)
// this will help to make code layout more cache friendly.
```

### Flags for aligning loops at 32 byte boundary
* -mllvm -align-all-nofallthru-blocks=5
* -mllvm -align-all-functions=5    
above will improve loop stream detector performance.
* uops will be fetched from cache.

### FACEBOOK BOLT will rearrange hot code together.

Farther reading:
https://easyperf.net/notes
https://lemire.me/en
https://www.agner.org/optimize/
https://epickrram.blogspot.com/lll