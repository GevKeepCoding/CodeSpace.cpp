#pragma once

#include <algorithm>
#include <numeric>
#include <vector>
// Here I will be training algorithms from standard library.


// accumulate
// reduce
// inner_product
// transform_reduce
// partial_sum
// exclusive_scan
// inclusive_scan

template <class _InIt, class _OutIt, class _Ty, class _BinOp>
_OutIt inclusive_scan(const _InIt _First, const _InIt _Last, _OutIt _Dest, _BinOp _Reduce_op, _Ty _Val)
{}
template <class _InIt, class _OutIt, class _Ty, class _BinOp, class _UnaryOp>
_OutIt transform_inclusive_scan(
    const _InIt _First, const _InIt _Last, _OutIt _Dest, _BinOp _Reduce_op, _UnaryOp _Transform_op, _Ty _Val)
{
    // compute partial noncommutative and associative transformed reductions including _Val into _Dest

    return _Dest;
}
template <class _InIt, class _OutIt, class _BinOp>
_OutIt inclusive_scan(const _InIt _First, const _InIt _Last, _OutIt _Dest, _BinOp _Reduce_op)
{}
template <class _InIt, class _OutIt, class _BinOp>
_OutIt partial_sum(const _InIt _First, const _InIt _Last, _OutIt _Dest, _BinOp _Reduce_op)
{}
template <class _InIt, class _OutIt, class _BinOp, class _UnaryOp>
_OutIt transform_inclusive_scan(
    const _InIt _First, const _InIt _Last, _OutIt _Dest, _BinOp _Reduce_op, _UnaryOp _Transform_op)
{
    // compute partial noncommutative and associative transformed reductions into _Dest

}
//
template <class _InIt, class _OutIt, class _Ty, class _BinOp>
_OutIt exclusive_scan(const _InIt _First, const _InIt _Last, _OutIt _Dest, _Ty _Val, _BinOp _Reduce_op)
{}
template <class _InIt, class _OutIt, class _Ty, class _BinOp, class _UnaryOp>
_OutIt transform_exclusive_scan(
	const _InIt _First, const _InIt _Last, _OutIt _Dest, _Ty _Val, _BinOp _Reduce_op, _UnaryOp _Transform_op)
{

}




//

template <class _InIt, class _OutIt, class _BinOp>
_OutIt adjacent_difference(const _InIt _First, const _InIt _Last, _OutIt _Dest, _BinOp _Func)
{
    // compute adjacent differences into _Dest

}

// partition


int main() {
	std::vector<int> v = { 1,23,4 };

	auto result = std::accumulate(v.begin(), v.end(), 0);
}