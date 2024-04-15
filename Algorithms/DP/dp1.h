#pragma once
import std;
using namespace std;



//https://leetcode.com/problems/last-stone-weight-ii/description/
class Solution1049 {
public:
    int lastStoneWeightII(std::vector<int>& stones) {
        int sum = std::accumulate(stones.begin(), stones.end(), 0);
        std::vector<bool> dp(sum / 2 + 1, false);
        dp[0] = true;

        for (int weight : stones) {
            for (int i = sum / 2; i >= weight; --i) {
                dp[i] = dp[i] || dp[i - weight];
            }
        }

        for (int i = sum / 2; i >= 0; --i) {
            if (dp[i]) {
                return sum - 2 * i;
            }
        }

        return 0;
    }
};