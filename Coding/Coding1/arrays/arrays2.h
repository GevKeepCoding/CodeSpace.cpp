#pragma once
import std;
using namespace std;





namespace arrays2
{
//Contiguous Array - Leetcode 525 - Python
//https://www.youtube.com/watch?v=agB1LyObUNE

/*
	525
	Given a binary array nums, return the maximum length of a contiguous sabarray
	with an equal number of 0 and 1.
*/
	// O(n^2)
	int FindMaxSubArrayLengthWithEqualZerosAndOnes(const vector<int>& nums) {
		int answer{0};
		int zeroes{0}, ones{0};
		int lo = 0;
		for (int lo = 0; lo < nums.size() - 1; ++lo)
			for (int hi = lo; hi < nums.size(); ++hi) {
				zeroes += nums[hi] == 0;
				ones += nums[hi] == 1;
				if (zeroes == ones and hi - lo + 1 > answer)
					answer = hi - lo + 1;
			}

		return answer;
	}
	// O(n)
	int FindMaxSubArrayLengthWithEqualZerosAndOnes2(const vector<int>& nums) {
		int zero{0}, one{0};
		int res{0};

		std::unordered_map<int, int> diff_index{};

		for (int i = 0; i < nums.size(); ++i) {
			const int& n = nums[i];
			if (n == 0) {
				zero++;
			}
			else {
				one++;
			}
			if (diff_index.contains(one - zero) == false)
				diff_index[one - zero] = i;
			else
				res = std::max(res, i - diff_index[one - zero]);
		}

		return res;
	}

	auto main() -> int
	{
		vector<int> nums{0, 1};
		cout << "Output:" << FindMaxSubArrayLengthWithEqualZerosAndOnes(nums);
		cout << "Output:" << FindMaxSubArrayLengthWithEqualZerosAndOnes(vector{1,0,0,1});
		cout << "Output:" << FindMaxSubArrayLengthWithEqualZerosAndOnes(vector{1,0,1,0,1,1});

		return 0;
	}


	class Solution2 {
	public:
		vector<int> successfulPairs1(vector<int>& spells, vector<int>& potions,
									 long long success) {
			std::vector<int> pairs(spells.size(), 0);
			int i = 0;
			for (int s : spells) {

				int goodPairs{0};
				int a = int((success + s - 1) / s);
				for (int p : potions)
				{
					if (p >= a) {
						++goodPairs;
					}
				}
				pairs[i++] = goodPairs;
			}
			return pairs;
		}

		vector<int> successfulPairs(vector<int>& spells, vector<int>& potions,
									long long success) {
			std::vector<int> pairs(spells.size(), 0);
			std::sort(potions.begin(), potions.end());
			int i = 0;
			for (int s : spells) {

				int goodPairs{0};
				int64_t a = (success + s - 1) / s;
				auto found = std::lower_bound(potions.begin(), potions.end(), a);

				if (found != potions.end()) {
					if (*found < a) found = std::next(found);
					goodPairs = std::distance(found, potions.end());
				}
				pairs[i++] = goodPairs;
			}
			return pairs;
		}
		auto main_example() -> int
		{
			Solution2 s;
			vector spells{5,1,3};
			vector potions{1,2,3,4,5};
			auto pairs = s.successfulPairs(spells, potions, 7);

			for (int i : pairs)
				cout << i << " ";

			return 0;
		}
	};


	class Solution1254LeetCode
	{
	public:

		bool dfs(vector<vector<int>>& grid, int i, int j) {
			bool res = true;
			size_t n = grid.size();
			size_t m = grid[0].size();
			if (i < 0 || i == n || j < 0 || j == m) return true;
			if (grid[i][j] == 1 || grid[i][j] == 2) return true;

			if (grid[i][j] == 0) {
				grid[i][j] = 2;
				if (i == 0 || i == n - 1 || j == 0 || j == m - 1)
				{
					res = false;
				}
			}
			
			bool res2 = dfs(grid, i - 1, j);
			bool res3 = dfs(grid, i + 1, j);
			bool res4 = dfs(grid, i, j - 1);
			bool res5 = dfs(grid, i, j + 1);
			return res && res2 && res3 && res4 && res5;
		}


		int closedIsland(vector<vector<int>>& grid) {
			size_t n = grid.size();
			size_t m = grid[0].size();
			int res = 0;
			for (int i = 1; i < n - 1; ++i) {
				for (int j = 1; j < m - 1; ++j) {
					if (grid[i][j] == 0)
					{
						res += dfs(grid, i, j);
					}
				}
			}
			return res;
		}
	};
	int main1254() {
		std::vector<vector<int>> v{{1,1,1,1,1,1,1,0},{1,0,0,0,0,1,1,0},{1,0,1,0,1,1,1,0},{1,0,0,0,0,1,0,1},{1,1,1,1,1,1,1,0}};

		arrays2::Solution1254LeetCode s;
		cout << "Number of closed Islands: " << s.closedIsland(v);
		return 0;
	}
	//LeetCode 1020. Number of Enclaves
	class Solution1020 {
	public:
		void dfs(vector<vector<int>>& grid, int i, int j) {
			auto m = grid.size();
			auto n = grid[0].size();
			if (i < 0 || j < 0 || i == m || j == n)
				return;
			if (grid[i][j] == 0)
				return;

			grid[i][j] = 0;
			dfs(grid, i - 1, j);
			dfs(grid, i, j - 1);
			dfs(grid, i + 1, j);
			dfs(grid, i, j + 1);
		}

		int numEnclaves(vector<vector<int>>& grid) {
			auto m = grid.size();
			auto n = grid[0].size();

			for (int i = 0; i < m; i++) {
				for (int j = 0; j < n; j++) {
					if (i == 0 || j == 0 || i == m - 1 || j == n - 1) {
						dfs(grid, i, j);
					}
				}
			}
			int result{0};
			for (auto row : grid) {
				result += std::accumulate(row.begin(), row.end(), 0);
			}
			return result;
		}
	};

	int main1020() {
		//[[0,0,0,0],[1,0,1,0],[0,1,1,0],[0,0,0,0]]
		//std::vector<vector<int>> grid{{0,0,0,0},{1,0,1,0},{0,1,1,0},{0,0,0,0}};
		//new data [[0],[1],[1],[0],[0]]
		std::vector<vector<int>> grid{{0},{1},{1},{0},{0}};
		Solution1020 s;
		cout << "enclaves" << s.numEnclaves(grid);
		return 0;
	}
}




namespace arrays2_1
{
	// const int N = 1e5 + 10;
	class Solution {
	public:
		Solution() { ios_base::sync_with_stdio(false); cin.tie(NULL); }
		// int p[n + 1];
		vector<int> p;
		int find(int x) {
			if (p[x] != x)
				p[x] = find(p[x]);
			return p[x];
		}
		int minScore(int n, vector<vector<int>>& roads) {
		  // int p[N];
		  // for(int i = 1; i <= n; ++i) p[i] = i;
			for (int i = 0; i <= n; ++i) p.push_back(i);
			for (auto& r : roads) {
				int a = find(r[0]), b = find(r[1]);
				if (a < b) swap(a, b);
				p[a] = b;
			}
			int res = numeric_limits<int>::max();
			for (auto& r : roads) {
				if (find(1) == find(r[0])) {
					res = min(res, r[2]);
				}
			}
			return res;
		}
	};
}

void main() {
	//[[1, 2, 9], [2, 3, 6], [2, 4, 5], [1, 4, 7]]
	std::vector<std::vector<int>> roads{{1, 2, 9}, {2, 3, 6}, {2, 4, 5}, {1, 4, 7}};
	arrays2_1::Solution s;
	cout << "minScore: " << s.minScore(4, roads);


}