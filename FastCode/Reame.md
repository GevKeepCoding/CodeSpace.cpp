TODO: Learn from this 2 books:
# High Performance Parallelism Pearls   | v1 2014, v2 2015
# Intel Xeon Phi Coprocessor High Performance Programming  |   in 2013, 2016


## [vcpkg-json reference](https://learn.microsoft.com/en-us/vcpkg/reference/vcpkg-json)






## Set associative caches

* We can fetch from RAM in 100-200 cycles
* We can fetch from L1 cache in 3-4 cycles
* We can fetch from L2 cache in 10-20 cycles
* We can fetch from L3 cache in 40-45 cycles (in some cases may take longer)

Example:
32 byte cache
2-way set associative => means each set has 2 cache lines
cache line size = 4 byte

---
each set size = 2 * 4 = 8 bytes

Number of sets = 32 / 8 = 4 sets

---
each Address == [Tag, Set Index, Block Offset]
Block Offset: = byte within a cache line
Set Index: = which set to look in
Tag: = Uniquely identifies the lines with the same Offset and Index

---
Intel skylike has L3 sets = 2048
L3 cache line size = 64 bytes

---

---