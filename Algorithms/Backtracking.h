#pragma once

#include <string>
#include <vector>

/*
Problem: Combination Sum

Given an array of distinct integers candidates and a target integer target, return a list of all unique combinations of candidates where
the chosen numbers sum to target. You may return the combinations in any order.
The same number may be chosen from candidates an unlimited number of times. Two combinations are unique if the frequency of at least
one of the chosen numbers is different.

Example 1:
Input: candidates = [2,3,6,7], target=7
Output: [[2,2,3], [7]]

*/

std::vector<std::vector<int>> combinationSum(std::vector<int> const& candidates, int target)
{
	std::vector<std::vector<int>> answer{};
	auto combinationHelper = [&]<typename Self>
		(this const Self& self, std::vector<int> const& candidates, int current_index, int current_sum
		, const std::vector<int>& current_combination, int target) {
			if (current_sum == target) {
				answer.push_back(current_combination);
				return;
			}
			if (current_sum > target) {
				return;
			}
			auto newCurrent_combination = current_combination;
			for (int i = current_index; i < candidates.size(); ++i) {
				newCurrent_combination.push_back(candidates[i]);
				self(candidates, i, current_sum + candidates[i], newCurrent_combination, target);
			}
		};
	combinationHelper(candidates, 0, 0, std::vector<int>{}, target);
	return answer;
}
