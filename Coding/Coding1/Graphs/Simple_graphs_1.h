#pragma once
import std;
using namespace std;

namespace simple_graphs1 {
	// LeetCode 1514. Path with Maximum Probability
	class Solution1514
	{
	public:
		double maxProbability(int n, vector<vector<int>>& edges, vector<double>& succProb, int start_node, int end_node) {
			std::map<int, std::vector<std::pair<int, double>>> g;
			for (int i = 0; i < edges.size(); ++i) {
				auto source = edges[i][0];
				auto dest = edges[i][1];
				g[source].push_back({dest, succProb[i]});
				g[dest].push_back({source, succProb[i]});
			}

			priority_queue<pair<double, int>> pq;
			pq.push({1.0 , start_node});

			set<int> visited;
			while (!pq.empty())
			{
				auto [prob, node] = pq.top();
				pq.pop();
				if (node == end_node)
					return -prob;
				visited.insert(node);

				for (auto [nei, edge_prob] : g[node])
				{
					if (visited.find(nei) == visited.end())
					{
						pq.push({prob * edge_prob, nei});
					}
				}
			}
			return 0;
		}
	};


	// test
	int main() {
		vector<vector<int>> edges = {{0, 1}, {1, 2}, {0, 2}};
		vector<double> succProb = {0.5, 0.5, 0.2};
		Solution1514 s;
		auto res = s.maxProbability(3, edges, succProb, 0, 2);
		cout << res << endl;
		return 0;
	}
}