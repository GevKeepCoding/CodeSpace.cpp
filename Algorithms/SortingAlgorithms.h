#pragma once
#include <experimental/generator>
#include <vector>
#include <algorithm>
#include <stack>



namespace sorting_algorithms
{
	using namespace std;
	void BubbleSort(vector<int>& vec) {
		for (size_t i = 0; i < vec.size(); i++)
		{
			for (size_t j = 0; j < vec.size() - i - 1; j++)
			{
				if (vec[j] > vec[j + 1]) {
					swap(vec[j], vec[j + 1]);
				}
			}
		}
	}

	void InsertionSort(vector<int>& vec)
	{
		for (size_t i = 1; i < vec.size(); i++)
		{
			int key = vec[i];
			int j = i - 1;
			while (j >= 0 && vec[j] > key) {
				vec[j + 1] = vec[j];
				j--;
			}
			vec[j + 1] = key;
		}
	}

	//3. Selection sort
	void SelectionSort(vector<int>& vec)
	{
		for (size_t i = 0; i < vec.size(); i++)
		{
			int minIndex = i;
			for (size_t j = i + 1; j < vec.size(); j++)
			{
				if (vec[j] < vec[minIndex]) {
					minIndex = j;
				}
			}
			swap(vec[i], vec[minIndex]);
		}
	}

	//4. Quick sort
	int Partition(vector<int>& vec, int low, int high)
	{
		int pivot = vec[high];
		int i = low - 1;
		for (size_t j = low; j < high; j++)
		{
			if (vec[j] < pivot) {
				i++;
				swap(vec[i], vec[j]);
			}
		}
		swap(vec[i + 1u], vec[high]);
		return i + 1;
	}

	void QuickSortRecursive(vector<int>& arr, int low, int high) {
		if (low < high) {
			int pivotIndex = Partition(arr, low, high);
			QuickSortRecursive(arr, low, pivotIndex - 1);
			QuickSortRecursive(arr, pivotIndex + 1, high);
		}
	}

	void QuickSortInteractive(vector<int>& arr, int low, int high)
	{
		stack<int> stack;

		stack.push(low);
		stack.push(high);

		while (!stack.empty())
		{
			high = stack.top();
			stack.pop();
			low = stack.top();
			stack.pop();

			int pivotIndex = Partition(arr, low, high);
			if (pivotIndex - 1 > low) {
				stack.push(low);
				stack.push(pivotIndex - 1);
			}
			if (pivotIndex + 1 < high) {
				stack.push(pivotIndex + 1);
				stack.push(high);
			}
		}
	}

	void Heapify(vector<int>& vec, int n, int i)
	{
		int largest = i;
		int left = 2 * i + 1;
		int right = 2 * i + 2;

		if (left < n && vec[left] > vec[largest]) {
			largest = left;
		}
		if (right < n && vec[right] > vec[largest]) {
			largest = right;
		}

		if (largest != i) {
			swap(vec[i], vec[largest]);
			Heapify(vec, n, largest);
		}
	}

	void heapSort(vector<int>& arr)
	{
		int n = arr.size();

		// Build a max-heap (rearrange elements)
		for (int i = n / 2 - 1; i >= 0; i--)
			Heapify(arr, n, i);

		// One by one extract elements from the heap
		for (int i = n - 1; i >= 0; i--)
		{
			// Move the current root (maximum element) to the end
			swap(arr[0], arr[i]);

			// Call heapify on the reduced heap
			Heapify(arr, i, 0);
		}
	}



	experimental::generator<int> TopKLargestElements(vector<int> vec, int k)
	{
		vector<int> heap(vec.begin(), vec.begin() + k);
		make_heap(heap.begin(), heap.end(), greater{}); // first element is the minimum

		if (k < vec.size()) {
			heap.push_back(vec[k]);
		}

		for (size_t i = k; i < vec.size(); i++)
		{
			if (heap.front() < vec[i]) {
				heap[k] = vec[i];
				push_heap(heap.begin(), heap.end(), greater{});
				pop_heap(heap.begin(), heap.end(), greater{});
			}
		}

		for (int i = 0; i < k; i++)
		{
			co_yield heap[i];
		}
	}

	vector<int> TopKLargestProd(vector<int> vec, int k) {
		nth_element(vec.begin(), vec.begin() + k, vec.end(), greater{});
		return vector<int>(vec.begin(), vec.begin() + k);
	}

	// Partially sorting an array.
	void exampleOfPartialSort(vector<int>& vec, int k) {
		std::partial_sort(vec.begin(), vec.begin() + k, vec.end(), greater{});
	}


}
