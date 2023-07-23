[LCU14-504: Taming ARMv8 NEON: from theory to benchmark results](https://www.youtube.com/watch?v=ixuDntaSnHI)

AArch64: 32 x 128-bit registers
AArch32: 16 x 128-bit registers

* **Ne10** maintained by **ARM**
  - has very good FFT
  - General math
  - Image processing
  - ...and growing

---
## Autovectorization

```c++
void vector_add_c_16x4_naive(uint16_t* vecr,
const uint16_t* veca,
const uint16_t* vecb)
{
 int i;
 for (i = 0; i < 4;++i)
	vecr[i] = veca[i] + vecb[i];
}

we need to change declaration to get better codegen:
void vector_add_c_16x4_naive(
uint16_t*		restrict vecr,
const uint16_t* restrict veca,
const uint16_t* restrict vecb);

// the other common trick is to tell the compiler that the loop iteration
count is the multiple of 4

void function(parameters, int count) {
	...
	for (int i =0; i < (count & ~3); ++i) {
	...
	}
	...
}


```
---

## To find why doesn't my loop vectorize?
* -ftree-vecotrizer-verbose=N where N=1,2,... to find out
More on: http://gcc.gnu.org/projects/tree-ssa/vectorization.html
* 
