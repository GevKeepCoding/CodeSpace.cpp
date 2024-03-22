#pragma once

#include<vector>
#include<string>



//Longest Substring Without Repeating Characters
int LongestSubstringWithoutRepeatingCharacters1(std::string s)
{
	std::vector<int> char_index(128, -1); // stores last index of each character
	int maxLen = 0, start = -1;
	for (int i = 0; i != s.length(); i++) {
		auto& c = char_index[s[i]];
		if (c > start)
			start = c;
		c = i;
		maxLen = std::max(maxLen, i - start);
	}
	return maxLen;
}

