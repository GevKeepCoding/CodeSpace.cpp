#pragma once


#include "Graph.h"


#include <set>
#include <queue>
#include <experimental/generator>
#include <map>

using namespace std::experimental;

using std::vector;
using std::pair;

namespace graph_algos
{

	generator<size_t> bfs(const vvGraph& graph, size_t root)
	{
		std::vector<bool> visited(graph.size(), false);
		visited[root] = true;
		std::queue<size_t> q;
		q.push(root);
		while (!q.empty())
		{
			auto current = q.front(); q.pop();
			co_yield current;
			for (auto& node : graph.adjacentNodesOf(current)) {
				if (!visited[node]) {
					q.push(node); visited[node] = true;
				}
			}
		}
	}

	namespace shortest_paths {
	/*
	Dijkstra's algorithm is a single-source shortest path algorithm that
	finds the minimum distance between a source vertex and every other
	vertex in a weighted graph. It uses a priority queue to select the minimum
	distance vertex from the set of vertices not yet included in the shortest path tree.
	The algorithm then updates the distances of its adjacent vertices.
	This process continues until the algorithm has included all vertices
	in the tree or if the destination vertex is included in the tree.
	* Should have non-negative edge weights.
	*/
	auto MinDistanceDijkstra(vvGraph const& graph, std::vector<std::vector<float>> dist, size_t start)
	{
		size_t count = graph.size();
		std::map<size_t, float> distances;
		for (size_t i = 0; i < count; ++i) {
			distances.insert({i, std::numeric_limits<float>::infinity()});
		}
		distances[start] = 0;

		std::set<size_t> visited{};

		while (visited.size() != count)
		{
			// find node with the smallest distance from the start node that has yet been visited
			size_t closest_vertex = 0; float min_distance = std::numeric_limits<float>::infinity();
			for (size_t vertex = 0; vertex < count; ++vertex) {
				if (!visited.contains(vertex)) {
					if (distances[vertex] < min_distance) {
						closest_vertex = vertex;
						min_distance = distances[vertex];
					}
				}
			}

			// for current_vertex check all it's neighbors and update their distances from
			// the start node, if necessary
			for (auto neighbor : graph.adjacentNodesOf(closest_vertex)) {
				if (!visited.contains(neighbor)) {
					auto newDistance = distances[closest_vertex] + dist[closest_vertex][neighbor];
					if (newDistance < distances[neighbor]) {
						distances[neighbor] = newDistance;
					}
				}
			}

			visited.insert(closest_vertex);
		}

		return distances;
	}


	


	}




	// Bipartite means that graph can be drawn with two colors and
	// any no edge connects nodes with same color.
	bool IsGraphBipartite(const vvGraph& graph)
	{
		enum Colors { NotColored = 0, Red, Green };
		std::vector<Colors> NodeColor(graph.size(), NotColored);
		// we will do bfs and color nodes if 2 neighbors have same color then graph is not bipartite
		NodeColor[0] = Red;
		for (auto node : bfs(graph, 0)) {
			for (auto adjacentNode : graph.adjacentNodesOf(node)) {
				if (NodeColor[node] == NodeColor[adjacentNode]) {
					return false;
				}
				else
				{
					NodeColor[adjacentNode] = (NodeColor[node] == Red ? Green : Red);
				}
			}
		}
		return true;
	}

	generator<size_t> dfs(const vvGraph& graph, size_t start, std::set<size_t>& visited)
	{
		visited.insert(start);

		co_yield start;
		for (auto next : graph.adjacentNodesOf(start)) {
			if (!visited.contains(next)) {
				dfs(graph, next, visited);
			}
		}
	}

	// Prim's algorithm is a greedy algorithm that finds a minimum spanning tree for a weighted 
	// undirected graph.
	// find the minimum weight edge and add it to spanning tree
	// find min edge for which one of it's vertices is in spanning tree and continue this step until done.
	

}