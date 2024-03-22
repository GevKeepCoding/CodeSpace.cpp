#pragma once
#include "Graphs\Graph.h"

import std;
using namespace std;


namespace graph_algos2
{
    // const int N = 1e5 + 10;
    // LeetCode 2492.
    class Solution {
    public:
        Solution() { ios_base::sync_with_stdio(false); cin.tie(nullptr); }
        // int p[n + 1];
        vector<int> p;
        int find(int x) {
            if (p[x] != x) p[x] = find(p[x]);
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


