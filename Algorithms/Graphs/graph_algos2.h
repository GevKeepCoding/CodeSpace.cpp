#pragma once
#include "Graphs\Graph.h"

import std;
using namespace std;


namespace graph_algos2
{
	class union_find
	{
		vector<int> parent;
		vector<int> rank;
	public:
		union_find(int n) : parent(n), rank(n, 1)
		{
			for (int i = 0; i < n; i++)
				parent[i] = i;
		}

		int find(int x)
		{
			if (parent[x] != x)
				parent[x] = find(parent[x]);
			return parent[x];
		}

		void unite(int x, int y)
		{
			auto p1 = find(x);
			auto p2 = find(y);
			if (p1 == p2)
				return;
			if (rank[p1] > rank[p2]) {
				parent[p2] = p1;
				rank[p1] += rank[p2];
			}
			else
			{
				parent[p1] = p2;
				rank[p2] += rank[p1];
			}
		}

		vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
			auto uf = union_find(static_cast<int>(accounts.size()));

			unordered_map<string, int> email_to_id;
			for (int i = 0; i < accounts.size(); i++)
			{
				for (int j = 1; j < accounts[i].size(); j++)
				{
					auto email = accounts[i][j];
					if (email_to_id.find(email) != email_to_id.end())
					{
						uf.unite(i, email_to_id[email]);
					}
					else
					{
						email_to_id[email] = i;
					}
				}
			}

			map<int, vector<string>> emailGroup;
			for (auto& [email, id] : email_to_id) {
				auto leader = uf.find(id);
				emailGroup[leader].push_back(email);
			}

			vector<vector<string>> res;
			for (auto& [id, emails] : emailGroup)
			{
				sort(emails.begin(), emails.end());
				vector<string> account;
				account.push_back(accounts[id][0]);
				account.insert(account.end(), emails.begin(), emails.end());
				res.push_back(account);
			}
			return res;
		}

	};


	class Solution2492 {
	public:
		Solution2492() { ios_base::sync_with_stdio(false); cin.tie(nullptr); }
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



	class Solution_Shortest_Alternating_Paths {
	public:
		vector<int> shortestAlternatingPaths(int n, vector<vector<int>>& redEdges,
											 vector<vector<int>>& blueEdges) {
			std::map<int, std::vector<int>> red, blue;

			for (auto& edge : redEdges) {
				red[edge[0]].push_back(edge[1]);
			}

			for (auto& edge : blueEdges) {
				blue[edge[0]].push_back(edge[1]);
			}

			vector<int> res(n, -1);

			queue<tuple<int, int, string>> q;
			q.push(tuple(0, 0, ""));
			std::set<pair<int, string>> visited;

			while (!q.empty()) {
				auto [node, lenght, prev_edge_color] = q.front(); q.pop();

				if (res[node] == -1) {
					res[node] = lenght;
				}

				if (prev_edge_color != string("RED")) {
					for (auto n : red[node]) {
						pair p(n, std::string("RED"));
						if (visited.find(p) == visited.end()) {
							visited.insert(p);
							q.push(tuple(n, lenght + 1, "RED"));
						}
					}
				}
				if (prev_edge_color != string("BLUE")) {
					for (auto n : blue[node]) {
						pair p(n, string("BLUE"));
						if (visited.find(p) == visited.end()) {
							visited.insert(p);
							q.push(tuple(n, lenght + 1, "BLUE"));
						}
					}
				}



			}
			return res;

		}
	};

	// testing

	int testing_Solution_Shortest_Alternating_Paths() {
		// rededges = [[0, 1], [1, 2]]
		vector<vector<int>> redEdges = {{0, 1}, {1, 2}};
		vector<vector<int>> blueEdges = {};
		Solution_Shortest_Alternating_Paths s;
		auto res = s.shortestAlternatingPaths(3, redEdges, blueEdges);
		for (auto r : res) {
			cout << r << " ";
		}
		cout << endl;
		return 0;
	}


	//--------------
	
}

using namespace graph_algos2;
//int main()
//{
//	
//	return 0;
//}