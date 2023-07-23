#include <iostream>


/*
			Performance issues

1. Indirect calls are expensive in kernel-mode due to speculative
execution mitigations (clearing branch predictor state).

2. Indirect calls are expensive because of CFI(Control-Flow-Integrity) checks (I.e. Control Flow Guard, Clang-CFI).

3. Test-and-branch's cost scales with the number of checks(can easily end up costing more than the function being optimized).

4. Making global state writeable, then read-only, is expensive(TLB shootdowns). Affects binary load time.




*/

#include <overridecapabilities.h> // OS Capabilities in windows sdk.
/*
Capabilities allow querying:
1. Specific CPU features or CPU model
2. Operating System Features
3. Anything else people need.🙂


Versioning: V1_CAPSET indicates OS Knows about all previous capabilities.

OVRDCAP_AMD64_V1_CAPSET

*/

//[[msvc::dispatch(qualifier:[all(AVX, ERMSB)] )]]
//void* my_memcopy(void*, void*, size_t)
//{
//	std::cout << "AVX, ERMSB";
//}
//
//[[msvc::dispatch(qualifier:[all(MEMCPY_ACCELERATOR)] )]]
//void* my_memcopy(void*, void*, size_t)
//{
//	std::cout << "MEMCPY_ACCELERATOR";
//}
//
//[[msvc::dispatch(map:
//[all(ERMSB, AVX2) -> my_memcpy_ermsb_avx2,
//all(ERMSB,AVX)-> my_memcpy_ermsb_avx,
//ERMSB-> my_memcpy_ermsb,
//AVX->my_memcpy_avx])]]
//void* my_memcpy2(void*, void*, size_t);
//

/*
void fun_avx512() {
	std::cout << "Fun avx";
}
void fun_avx2() {
	std::cout << "fun avx2";
}


[[msvc::dispatch(map: [AVX512 -> fun_avx512, AVX2 -> fun_avx2]) ]]
void fun() { return; }




auto main_avx() -> int
{
	//OVRDCAP_AMD64_V1_CAPSET
	fun();

	return 0;
}

*/