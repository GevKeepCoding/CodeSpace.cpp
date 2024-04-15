## Magical Compiler flags

### Shared libraries

#### Optimization flags for shared libraries

* -fvisibility=hidden - This will hide all symbols by default, except those explicitly marked with __attribute__((visibility("default"))).
* -fvisibility-inlines-hidden - This will hide inline functions by default, except those explicitly marked with __attribute__((visibility("default"))).
**Below 2 options can give performance improvement of 10-20% in some libs**.
[**Юрий Грибов — Динамические библиотеки и способы ускорения их работы**](https://www.youtube.com/watch?v=blQavgcwrpA&list=TLPQMzAwNTIwMjUmgWkIMRRuNg&index=5)

* -Bsymbolic-functions - This will make it possible to make faster calls to functions defined in the same shared library, instead of going through the global symbol table. 
* -fno-semantic-interposition - This will prevent the compiler from generating code that allows a shared library to be replaced at runtime with a different version that has the same symbol names. used in GCC with -Ofast flag.

* -Wl,--as-needed - This will make the linker not link against libraries that are not needed by the shared library. this will reduce the size of the shared library and improve performance.


**Improve dll load time** - For security reasons, ASLR doesn't allow this kind of optimization.
* Windows: preferred load address, /BASE flag
* Linux: Prelink
* **Disable Delay Loading** - -fno-plt -
  * will make library function calls faster.
  * Will decrease I cache usage.
  * Will increase library load time as all addresses are resolved at the program start.


* -Wl,-z,relro - is used by default on RHEL/Fedora and Ubuntu.

---





---

### Gdb

```
# shortcuts to see disassembly in ui mode.
Ctrl+x, a then Ctrl+x, 2  then if you want Ctrl+x, 2
```



### Valigrind

* You can see L1 and last level cache misses.
1. sudo valgrind --main-stacksize=189777216 --tool=callgrind
--cache-sim=yes --branch-sim=yes --dump-instr=yes --collect-jumps=yes ./app

### perf tools


---
* perf stat -e cache-references,cache-misses,L1-dcache-prefetches,instructions,cpu-cycles,branches,branch-misses,duration_time ./app
* 


---
* perf stat -e cycles,instructions,l1d-loads,l1d-load-misses,branches,branch-misses
* perf record -e cycles,instructions,l1d-loads,l1d-load-misses,branches,branch-misses --cal-graph=lbr
     --cal-graph has 3 options.

* to improve accuracy use :p :pp :ppp 
* perf record -b program
* perf record --call-graph lbr -j any_call,any_ret program
* perf record -e intel_pt//u program

* perf report --no-children
* 

**Conclusion from perf stat**
* **If instructions per cycle is > 4 it's good, if < 1 it's bad.**

---
https://www.youtube.com/watch?v=rglmJ6Xyj1c&list=TLPQMDEwMTIwMjXc4frbjEmdNg&index=6
** perf stat -a --topdown sleep 1

if retiring is 100% we can't do better.

---





```bash
#! /bin/bash
export VEC=avx2
#export VEC=avx
#export VEC=sse
#export VEC=mmx
export CXX="g++ --std=c++17 -Wno-unused-local-typedefs -Wno-ignored-attributes"
#export CXX=g++
FILE=${1/.C/}
#FILE_BM==`echo $FILE | sed -r -e 's/([0-9])+/_bm/'`
$CXX $FILE_BM.C $FILE.C -I../src/gbench/include -g -O4 -I. -Wall -Wextra -Werror -fabi-version=6 -pedantic -m$VEC ../src/gbench/lib/libbenchmark.a \
-lpthread -lrt -lm -o $FILE
```

---
### transparent huge pages

* Default page size is still 4k, but processes can **madvise()** use of large pages
* echo madvise > /sys/kernel/mm/transparent_hugepages/enabled

* To defer instead of blocking large page requests
* echo defer > /sys/kernel/mm/transparent_hugepage/defrag  

---
### Cache lines, loads and stores

* The unit of granularity of a cache entry is 64bytes(512 bits)
* Cache lines can be in different states:
  * M - exclusively owned by that core and modified(dirty)
  * E - exclusively owned by that core but not modified
  * S - shared read-only with other cores
  * I - invalid, cache line not used


---
### Core dump collection:

```bash
cat /proc/sys/kernel/core_pattern  


ulimit -c unlimited  

./a.out

sudo coredumpctl list  

# from above command get the process id you want.
sudo touch core.dump

sudo coredumpctl dump 3633400 -o core.dump

gdb ./a.out core.dump

```

---


