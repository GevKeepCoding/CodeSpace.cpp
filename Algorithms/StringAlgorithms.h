#pragma once

#include<vector>
#include<string>



//Longest Substring Without Repeating Characters
int LongestSubstringWithoutRepeatingCharacters1(std::string s)
{
	std::vector<int> charMap(256, -1); // stores last index of each character
	int maxLen = 0, start = -1;
	for (int i = 0; i != s.length(); i++) {
		if (charMap[s[i]] > start)
			start = charMap[s[i]];
		charMap[s[i]] = i;
		maxLen = std::max(maxLen, i - start);
	}
	return maxLen;
}

int LongestSubstringWithoutRepeatingCharacters2(std::string s) {
	int l = 0, r = 0;
	int maxLen = 0;
	for (; r < s.size(); r++) {
		if (s[l] == s[r]) {
			l++;
		}
		else {
			maxLen = std::max(maxLen, r - l + 1);
		}
	}
	return maxLen;
}