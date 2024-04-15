#pragma once

#include <algorithm>
#include <xmmintrin.h>

// Optimizing Binary Search - Sergey Slotin - CppCon 2022 - https://www.youtube.com/watch?v=1RIPMQQRBWk

// initial lower_bound
int lower_bound(int* a, int n, int x)
{
	int l = 0, r = n;
	while (l < r) {
		int m = (l + r) / 2;
		if (a[m] < x) l = m + 1;
		else r = m;
	}
	return l;
}
// stl lower_bound
template<class fw, class t, class pr>
fw lower_bound(fw first, fw last, const t& val, pr pred)
{
	std::iter_difference_t<fw> count = std::distance(first, last);

	while (0 < count)
	{
		const std::iter_difference_t<fw> count2 = count / 2;
		const auto mid = std::next(first, count2);
		if (pred(*mid)) {
			first = std::next(mid);
			count -= count2 + 1;
		}
		else
		{
			count = count2;
		}
	}
	return first;
}
/*

fetch - fetches the instruction
decode - decodes the instruction
---
execute - executes the instruction
memory - read/write
write - write back to register


*/

int lower_bound2(int* t, int n, int x) {
	int* base = t, len = n;
	while (len > 1) {
		int half = len / 2;
		if (base[half - 1] < x) {
			base += half;
			len = len - half; // = ceil(len / 2)
		}
		else
		{
			len = half; // = floor(len / 2)
		}
	}
	return *base;
}

int lower_bound3(int* t, int n, int x) {
	int* base = t, len = n;
	while (len > 1) {
		int half = len / 2;
		if (base[half - 1] < x)
			base += half;
		len -= half;
	}
	return *base;
}// we will do exactly log2(n) iterations.
// this is not the same as std::lower_bound
// we will do more iterations on average but 
// we know that we will be doing log2(n) iterations
//=> this means we now have predicatability
int lower_bound4(int* t, int n, int x) {
	int* base = t, len = n;
	while (len > 1) {
		int half = len / 2;
		base += (base[half - 1] < x) * half;//will be replaced by cmov
		len -= half;
	}
	return *base;
}// 2-3x faster for smaller arrays, but for larger ones
// perf is worse than on normal one
int lower_bound5(int* t, int n, int x) {
	int* base = t, len = n;
	while (len > 1) {
		int half = len / 2;
		len -= half;
		/*__builtin_prefetch(&base[len / 2 - 1]);
		__builtin_prefetch(&base[half + len / 2 - 1]);*/
		_mm_prefetch((char*)&base[len / 2 - 1], _MM_HINT_T0);
		_mm_prefetch((char*)&base[half + len / 2 - 1], _MM_HINT_T0);
		base += (base[half - 1] < x) * half;//will be replaced by cmov
	}
	return *base;
}// slightly degrades perf for smaller sizes compared to above one,
// but for larger sizes it is faster
// with 4 prefetches, it is faster than the this one for larger sizes and
// slower for smaller sizes.

namespace eytzinger
{
	/*
		
		We can run BFS on a binary tree and construct eytzinger array.
		Used in heaps, segment trees, etc.
		this new array has much better temporal locality
		than sorted vector for binary search.
	*/


	int a[n];
	// t is from 1 to n that's why we use n+1
	alignas(64) int t[n + 1]; // alignas is really important to be sure that
	// we will prefetch the right data
	
	void eytzinger(int* t, int k = 1) {
		static int i = 0;
		if (k <= n) {
			eytzinger(t, 2 * k);
			t[k] = a[i++];
			eytzinger(t, 2 * k + 1);
		}
	}
	// this way sorted array can be permuted on O(n) time.

	int search(int* t, int n, int x) {
		int k = 1;
		while (k <= n) {
			k = 2 * k + (t[k] < x);
		}
		return t[k / 2];
	}
	/*
		The search result is not the lower_bound.
		example:
		array: 0 1 2 3 4 5 6 7 8 9
		eytzinger: 6 3 7 1 5 8 9 0 2 4
		after comparing with 3: we go one left, and then keep going 'right'
		now we can look at binary representation of found k and find lower_bound position from it.
		going left == multiply by 2 ==> adding 0 the the end of it's binary representation
		going right== *2 + 1 ==> adding 1 the the end of it's binary representation
		
		let's remove the longest suffix of 1s from k


		*/

	int lower_bound(int* t, int n, int x) {
		int k = 1;
		while (k <= n) {
			k = 2 * k + (t[k] < x);
		}

		//k >>= __builtin_ffs(~k);//find first set bit

		/*k /= 2;
		while (k & (k - 1)) {
			k &= k - 1;
		}*/
		return t[k];
	}// on smaller arrays it's perf is on par with branchless binary search
	// but on larger ones it's slowest one.


	int lower_bound(int* t, int n, int x) {
		int k = 1;
		while (k <= n) {
			//__builtin_prefetch(&t[k * 2]);
			//__builtin_prefetch(&t[k * 4]);
			//__builtin_prefetch(&t[k * 8]);
			//__builtin_prefetch(&t[k * 16]);// 4 iterations later
			k = 2 * k + (t[k] < x);
		}

		//k >>= __builtin_ffs(~k);//find first set bit
		return t[k];
	}
	/*
above code is commented because of msvc
	
this code is 2x faster for larger data than the std::lower_bound
there is however one drawback:
we trade of bandwidth for latency with prefetching
This doesn't work well in bandwidth constrained environments(e.g., multi-threading)


We can fetch fewer cache lines by changing to B-TREES
B >= 2 keys in each node.

	*/







}