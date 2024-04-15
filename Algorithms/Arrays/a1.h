#pragma once
#include <algorithm>
import std;
using namespace std;

namespace array_tips
{

int Longest_increasing_subsequence(vector<int>& nums)
{
    vector<int> piles;

    for (int n : nums) {
        auto it = lower_bound(piles.begin(), piles.end(), n);
        if (it == piles.end()) {
            piles.push_back(n);
        }
        else {
            *it = n;
        }
    }
    return piles.size();
}// n*log(n)



// p1={a1,b1} p2 = {a2,b2} if b1 < a2 => they create chain.
int findLongestChain(std::vector<std::vector<int>>& pairs) {
	std::ranges::sort(pairs, {}, [](auto& a) { return a[1]; });

	int length = 1;
	int end = pairs[0][1];

	for (int i = 1; i < pairs.size(); i++) {
		if (end < pairs[i][0]) {
			length++;
			end = pairs[i][1];
		}
	}

	return length;
}


}

void main_longest_increasing_subseq()
{
    vector<int> nums = { 10, 9, 2, 5, 3, 7, 101, 18 };
    int res = array_tips::Longest_increasing_subsequence(nums);
    cout << res << endl;
}