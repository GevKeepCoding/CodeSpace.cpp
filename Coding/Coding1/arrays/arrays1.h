#pragma once
/*
N:2962. Count Subarrays where max element appears at least K times

Return the number of subarrays where
the max element of nums appears at least k times
in that contiguous subarray.

*/

import std;

//Count Subarrays Where Max Element Appears at Least K Times - Leetcode 2962 - Python https://www.youtube.com/watch?v=CZ-z1ViskzE
// one solution is to find all subarrays were the end is at r point.
int countSubarrays(const std::vector<int>& nums, int k) {

	int max_n = *std::max_element(nums.begin(), nums.end());
	int max_cnt = 0;
	int l = 0;
	int res = 0;

	for (int r = 0; r < nums.size(); ++r) {
		if (nums[r] == max_n)
			max_cnt += 1;

		while (max_cnt > k or (l <= r and max_cnt == k and nums[l] != max_n))
		{
			if (nums[l] == max_n)
				max_cnt -= 1;
			l += 1;
		}
		if (max_cnt == k)
			res += l + 1;
	}

	return res;
}

int countSubarrays_optimized(const std::vector<int>& nums, int k) {

	int max_n = *std::max_element(nums.begin(), nums.end());
	int max_cnt = 0;
	int l = 0;
	int res = 0;

	for (int r = 0; r < nums.size(); ++r)
	{
		if (nums[r] == max_n)
			max_cnt += 1;

		while (max_cnt == k)
		{
			if (nums[l] == max_n)
				max_cnt -= 1;
			l += 1;
		}
		res += l;
	}

	return res;
}


///
namespace google_kickstart
{

import std;
	using namespace std;

	// number of hills (the element is a hill if it's bigger then it's left and right neighbors) (elements at the edges aren't hills)
	//int N;
	//int arr[110];
	//
	//void gogo() {
	//	cin >> N;
	//	for (int i = 0; i < N; i++) {
	//		cin >> arr[i];
	//	}
	//	int ans = 0;
	//	for (size_t i = 1; i < N - 1; i++)
	//		if (arr[i] > arr[i - 1] && arr[i] > arr[i + 1])
	//			ans++;
	//	cout << ans;
	//}
	//
	//
	//int main() {
	//	ios_base::sync_with_stdio(0);
	//
	//	int T; cin >> T;
	//	for (size_t i = 0; i < T; i++)
	//	{
	//		cout << "Case #" << i + 1 << ": ";
	//		gogo();
	//	}
	//}

	using ll = long long;

	int n;
	ll D;
	ll x[1010];

	// N busses, you should go from 1 to Nth.
	// ith buss works only on every x[i]th day.
	// you should go through all busses.
	// question is the max day when you can start your travel to be able to complete not later than D_th day.
	bool works(ll k) {
		for (int i = 0; i < n; ++i)
		{
			if (k % x[i])
				k += (x[i] - k % x[i]);
		}
		return k <= D;
	}


	void gogo() {
		cin >> n >> D;
		for (int i = 0; i < n; ++i)
			cin >> x[i];

		ll lo = 0, hi = D;
		while (lo < hi)
		{
			ll mid = (lo + hi + 1) / 2;
			if (works(mid))
				lo = mid;
			else
				hi = mid - 1;
		}
		cout << lo << "\n";
	}



	int main() {
		ios_base::sync_with_stdio(0);

		int T; cin >> T;
		for (size_t i = 0; i < T; i++)
		{
			cout << "Case #" << i + 1 << ": ";
			gogo();
		}
	}
}