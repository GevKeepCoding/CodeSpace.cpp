#pragma once

// Eduardo Madrid
//Using Integers as Arrays of Bitfields a.k.a.SWAR Techniques - Eduardo Madrid - CppCon 2019 https://www.youtube.com/watch?v=H-bm71KmYTE


/*
AVX1 only supports floating points. AVX2 supports integers.



He demonstrates how we can make use of simd instead of popcnt to get faster result (clang worked and gcc not on his example)



SIMD - x86-64 SSE
* Supports saturation arithmetic for 128 vectors and element sizes of 8, 32, 64 bits. When
  Overflow occurs, the operation does not affect other elements.

* Incosistency: There are pmovmskb and movemksps, movmskpd, they pick the most significant bit of: 16 bytes in the SSE
register, or 4 single precision or 2 double precision. What about 8 16-bit? (there is no instruction for that)

* The SWAR comes to the rescue



https://youtu.be/H-bm71KmYTE?t=987
https://github.com/lattera/glibc/blob/master/string/strlen.c


https://lemire.me/blog/2018/10/03/quickly-parsing-eight-digits/




*/