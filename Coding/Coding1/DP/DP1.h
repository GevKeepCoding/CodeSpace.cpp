#pragma once
import std;
using namespace std;



namespace dynamic_programming
{
// is it possible to get the sum equal to the total
bool knapsack_01(vector<int>& weights, int W)
{
	vector<bool> can(W + 1, false);
	can[0] = true;
	for (int i = 0; i < weights.size(); i++) {
		auto x = weights[i];
		for (int i = W; i >= x; i--) {
			if (can[i - x]) can[i] = true;
		}
	}
	return can[W];
}
// Now let's write optimized version with bitsets
bool knapsack_01_optimized(vector<int>& weights, int W)
{
	const int max_w = 1000;
	bitset<max_w> can; // total+1 <= 1000
	can[0] = true;
	for (auto x : weights) {
		can |= (can << x);
	}
	return can[W];
}

}

