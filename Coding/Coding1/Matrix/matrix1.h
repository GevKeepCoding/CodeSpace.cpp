#pragma once
import std;
using namespace std;

namespace matrix1 {

    class Solution {
    public:
        int maxDistance(vector<vector<int>>& grid) {
            auto n = grid.size();
            queue<pair<int, int>> q;
            for (int r = 0; r < n; r++) {
                for (auto c = 0; c < n; ++c) {
                    if (grid[r][c]) {
                        q.push(pair{r, c});
                    }
                }
            }

            auto res = -1;
            vector<pair<int, int>> direct = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
            while (!q.empty()) {
                auto [r, c] = q.front();
                q.pop();

                res = grid[r][c];
                cout << "r,c" << r << " " << c << endl;
                for (auto [dr, dc] : direct) {
                    auto R = r + dr;
                    auto C = c + dc;
                    if (min(R, C) >= 0 and max(R, C) < n
                        and grid[R][C] == 0) {
                        q.push({R,C});
                        grid[R][C] = grid[r][c] + 1;
                    }
                }
            }
            if (res > 1) return res - 1;
            else return -1;

        }
    };


    int main() {
        //grid = [[1,0,1],[0,0,0],[1,0,1]]
        vector<vector<int>> grid = {{1,0,1}, {0,0,0}, {1,0,1}};
        Solution s;
        auto res = s.maxDistance(grid);
        return res;
    }
}