#pragma once
#include "../Helper/simplify.h"

import std;
using namespace std;




namespace strings1
{
	/*
		Removing Stars From a String - Leetcode 2390 - Python
		https://www.youtube.com/watch?v=pRyFZIaKegA&list=PLQpVsaqBj4RLwXMZ9LaAFf4rVowiC3ZcG

		You are given a string s, which contains *.
		in one operation you can:
		* choose a star in s.
		* Remove the closest non-star character in its left,
		* as well as remove the star itself.
		*
		Return the string after all stars have been removed.

		Node:
		Input will be generated such that the operation is always possible.
		It can be shown that the resulting string will always be unique.
		Example1:
		Input: s = "leet**cod*e"
		Output: "lecoe"
	*/

	std::string RemoveStarsWithLeftNonStar(std::string input) {

		std::string res;
		int count_star{0};
		for (int i = int(input.length()) - 1; i >= 0; i--)
		{
			if (input[i] == '*')
			{
				count_star++;
			}
			else
			{
				if (count_star > 0) {
					--count_star;
					continue;
				}
				res.push_back(input[i]);
			}
		}

		return std::string(res.rbegin(), res.rend());
	}


	int main()
	{
		std::string input = "leet**cod*e";
		std::string output = strings1::RemoveStarsWithLeftNonStar(input);
		std::cout << output << std::endl;
		return 0;
	}

	//------------------------------
	// 2439. Minimize Maximum of Array
	//3,7,1,6
	class Solution1 {
	public:
		int minimizeArrayValue(std::vector<int>& nums) {
			int res, total;
			res = total = nums[0];

			for (int i = 1; i < nums.size(); ++i) {
				total += nums[i];
				res = std::max(res, int(std::ceil((double)total / (i + 1))));
			}
			return res;
		}//74ms

		int minimizeArrayValue2(vector<int>& nums) {
			ios_base::sync_with_stdio(false);
			cin.tie(nullptr);
			cout.tie(nullptr);

			double ans = 0, sum = 0, tempSum = 0, numElements = 0;

			for (int i = 0; i < nums.size(); ++i) {
				sum += nums[i];
				tempSum += nums[i];
				numElements += 1;
				if (tempSum >= numElements * ans) {
					ans = ceil(sum / (i + 1));
					tempSum = 0;
					numElements = 0;
				}
			}
			return int(ans);
		} //faster 43ms
	};
	
	void Solution_example() {
		vector v{13,13,20,0,8,9,9};
		strings1::Solution1 s;
		int res = s.minimizeArrayValue(v);
		cout << res << endl;
	}

	// LeetCode
	// 2405. Optimal Partition of String

	int partitionStringSlooow(string s) {
		int res{1};
		std::set<char> chars;

		for (char c : s) {
			if (!chars.contains(c))
			{
				chars.insert(c);
			}
			else {
				++res;
				chars.clear();
				chars.insert(c);
			}
		}
		return res;
	}//100ms

	static const int _ = []() {
		ios_base::sync_with_stdio(false);
		cin.tie(NULL);
		cout.tie(NULL);

		return 0;
	}();

	class Solution2 {
	public:
		int partitionString(string s) {
			int ans = 0;

			long long bit = 0;
			int letter = 0;
			for (auto& i : s)
			{
				letter = 1 << (i - 'a');
				if (bit & letter)
				{
					ans++;
					bit = 0;
				}

				bit = bit | letter;
			}

			return ans + 1;
		}
	};//3ms

}



