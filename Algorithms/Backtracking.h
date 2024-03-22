#pragma once

import std;


namespace backtracking {

	using namespace std;




	/*
	Problem: Combination Sum

	Given an array of distinct integers candidates and a target integer target, return a list of all unique
	combinations of candidates where the chosen numbers sum to target.
	You may return the combinations in any order.
	The same number may be chosen from candidates an unlimited number of times.

	Two combinations are unique if the frequency of at least one of the chosen numbers is different.

	Example 1:
	Input: candidates = [2,3,6,7], target=7
	Output: [[2,2,3], [7]]

	*/

	auto Unique_combinations_with_sum_target(const vector<int>& coll, int target)
	{
		vector<vector<int>> answer{};

		auto combinationHelper = [&]<typename Self>
			(this const Self & self, vector<int> const& coll, int current_index, int current_sum,
			 const vector<int>&current_combination) {

			if (current_sum == target) { answer.push_back(current_combination); return; }

			if (current_sum > target) { return; }

			for (int i = current_index; i < coll.size(); ++i) {
				auto newCurrent_combination = current_combination;
				newCurrent_combination.push_back(coll[i]);
				self(coll, i, current_sum + coll[i], newCurrent_combination);
			}
		};

		combinationHelper(coll, 0, 0, vector<int>{});
		return answer;
	}

	// Now when order is important
	//[2,2,3] , [2,3,2], [3,2,2] [7] are all different
	auto Unique_ordered_combinations_with_sum_target(const std::vector<int>& coll, int target)
	{
		std::vector<std::vector<int>> answer{};
		auto combinationHelper = [&]<typename Self>
			(this const Self & self, std::vector<int> const& coll, int current_sum
			 , const std::vector<int>&current_combination) {
			if (current_sum == target) {
				answer.push_back(current_combination);
				return;
			}
			if (current_sum > target) {
				return;
			}
			for (int i = 0; i < coll.size(); ++i) {
				auto newCurrent_combination = current_combination;
				newCurrent_combination.push_back(coll[i]);
				self(coll, current_sum + coll[i], newCurrent_combination);
			}
		};
		combinationHelper(coll, 0, std::vector<int>{});
		return answer;
	}


	// return all possible permutations of k elements from a vector of unique integers
	// subset elements can not be repeated
	vector<vector<int>> all_permutations_of_k_elements(const vector<int>& nums, int k)
	{
		vector<vector<int>> res;

		vector<int> permutation;
		permutation.reserve(k);

		vector<bool> used(nums.size(), false);

		auto all_permutations_helper = [&](this auto& self)
		{
			if (permutation.size() == k) {
				res.push_back(permutation);
				return;
			}

			for (int i = 0; i < nums.size(); ++i)
			{
				if (!used[i]) { // valid choice

					// make a choice
					used[i] = true;
					permutation.push_back(nums[i]);

					self();

					// undo the choice
					used[i] = false;
					permutation.pop_back();
				}
			}
		};

		all_permutations_helper();

		return res;
	}


	// we have a set with unique elements
	// subset elements can be repeated
	//generate all subsets of all sizes of the set { {}, {a1}, ... }
	vector<vector<int>> all_subsets_of_set(vector<int>& nums) {
		vector<vector<int>> result;

		vector<int> subset{};
		vector<bool> visited(nums.size(), false);
		auto all_subsets_helper = [&](this auto& self) -> void {
			result.push_back(subset);

			if (subset.size() == nums.size()) return;

			for (int i = 0; i < nums.size(); ++i)
			{
				subset.push_back(nums[i]);

				self();

				subset.pop_back();
			}
		};

		all_subsets_helper();


		_ASSERT(result.size() == (pow(nums.size(), nums.size() + 1) - 1) / (nums.size() - 1));
		return result;
	}

	int main()
	{
		vector<int> nums{2,3,7};
		auto result = Unique_ordered_combinations_with_sum_target(nums, 7);
		auto result = Unique_combinations_with_sum_target(nums, 7);
		for (auto& v : result) {
			for (auto& e : v)
				std::cout << e << ' ';
			cout << '\n';
		}

		return 0;
	}

}