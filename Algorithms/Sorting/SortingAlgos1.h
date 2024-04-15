#pragma once
import std;
using namespace std;

namespace sorting_algos1
{

	template<class fw,class pr>
	fw partition(fw first, fw last, pr pred)
	{
		for (;;) {
			for (;;) {
				if (first == last)
					return first;

				if (!pred(*first))
					break;
				++first;
			}
			do
			{
				--last;
				if (first == last) {
					return first;
				}
			} while (!pred(*last));

			std::swap(*first, *last);
			++first;
		}
	}

	int hoar_partition(int arr[], int low, int high) {
	/**
	 * Hoare's partition scheme for the quicksort algorithm.
	 */
		int pivot = arr[low]; // Choose the first element as the pivot
		int i = low - 1;
		int j = high + 1;

		while (true) {
			// Move i pointer to the right until we find an element greater than or equal to the pivot
			do {
				i++;
			} while (arr[i] < pivot);

			// Move j pointer to the left until we find an element less than or equal to the pivot
			do {
				j--;
			} while (arr[j] > pivot);

			// If i and j cross, we're done
			if (i >= j) {
				return j;
			}

			// Swap elements at i and j
			std::swap(arr[i], arr[j]);
		}
	}

	int partition(int* a, int l, int r) {
		auto x = a[r];
		auto i = l - 1;
		for (int j = l; j < r; j++) {
			if (a[j] <= x) {
				i += 1;
				std::swap(a[i], a[j]);
			}
		}
		std::swap(a[i + 1], a[r]);
		return i + 1;
	}


	void quick_sort(int* a, int p, int r) {
		if (p < r) {
			auto q = partition(a, p, r);
			quick_sort(a, p, q - 1);
			quick_sort(a, q + 1, r);
		}
	}

    void quick_sort_tail_call(int* a, int p, int r) {
        while (p < r) {
            auto q = partition(a, p, r);
            quick_sort_tail_call(a, p, q - 1);
            p = q + 1;
        }
    }
	//---
	
	//---
	// insertion sort
	template<class bid, class pr>
	bid insertion_sort(bid first, bid last, pr pred)
	{
		if (first != last) {
			for (bid mid = first; ++mid != last;) {
				bid hole = mid;
				std::iter_value_t<bid> val(std::move(*mid));

				if (pred(val, *first)) {
					std::move_backward(first, mid, ++hole);
					*first = std::move(val);
				}
				else { // look for insertion point
					for (bid prev = hole; pred(val, *--prev); hole = prev) {
						*hole = std::move(*prev);
					}
					*hole = std::move(val);//insert element in hole
				}
			}
		}
		return last;
	}

	void insertion_sort2(auto first, auto last) {
		for (auto i = first; i != last; ++i) {
			std::rotate(std::upper_bound(first, i, *val), i, i + 1);
		}
	}

	// top 5 sorting algorithms
	// merge_sort, quick_sort

	// selection sort
	template<class bid, class pr>
	bid selection_sort(bid first, bid last, pr pred)
	{
		for (bid i = first; i != last; ++i) {
			bid min = i;
			for (bid j = std::next(i); j != last; ++j) {
				if (pred(*j, *min)) {
					min = j;
				}
			}
			if (min != i) {
				std::iter_value_t<bid> temp = std::move(*i);
				*i = std::move(*min);
				*min = std::move(temp);
			}
		}
		return last;
	}


	void bubble_sort1(auto first, auto last) {
		for (auto i = first; i != last; ++i) {
			for (auto j = first; j < i; ++j) {
				if (*i < *j) {
					std::iter_swap(i, j);
				}
			}
		}
	}
	// bubble sort
	template<class bid, class pr>
	bid bubble_sort(bid first, bid last, pr pred)
	{
		if (first == last) return;
		--last;
		bool swapped = true;
		while (swapped)
		{
			swapped = false;
			for (bid i = first; i != last; ++i) {
				bid next = std::next(i);
				if (pred(*next, *i)) {
					// swap
					std::iter_swap(next, i);
					swapped = true;
				}
			}
		}
	}

	// heap_sort
	template<class RanIt, class Pr>
	void HeapSort(RanIt first, RanIt last, Pr pred)
	{
		std::make_heap(first, last, pred);
		std::sort_heap(first, last, pred);
	}

	// merge sort
	void merge_sort(auto first, auto last) {
		if (last - first > 1) {
			auto middle = first + (last - first) / 2;
			merge_sort(first, middle);
			merge_sort(middle, last);
			std::inplace_merge(first, middle, last);
		}
	}
}

/*

static void bm_sortbase(benchmark::State& state, auto start)
{
	auto len = state.range(0);
	auto dist = std::uniform_int_distribution(-100000, 100000);
	const auto v0 = get_random_vec(len, seed, dist);

	auto v = v0;

	for (auto _ : state) {
		v = v0;
		sort(b.begin(), v.end());
		benchmark::ClobberMemory();
	}
	if (!std::is_sorted(v.begin(), v.end()))
		throw std::runtime_error("Bad Sort");

	auto bytes = len * sizeof(int);
	state.SetBytesProcessed(bytes * state.iterations());
	state.counters["bytes_used"] = benchmark::Counter(bytes, benchmark::Counter::kDefaults,
													  benchmark::Counter::oneK::kIs1024);


}

*/



//int main() {
//
//	std::vector v{12,3,4};
//	std::sort(v.begin(), v.end(), std::less<int>());
//
//
//	for (auto e : v) {
//		cout << e << " , ";
//	}
//}