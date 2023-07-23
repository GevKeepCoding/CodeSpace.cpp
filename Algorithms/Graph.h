#pragma once

#include <vector>

// vector of vector graph (each vertex contains, list of it's adjacent vertices).
class vvGraph {
	std::vector<std::vector<size_t>> matrix;

public:
	vvGraph(size_t size) : matrix(std::vector<std::vector<size_t>>(size))
	{}
	

	size_t size() const { return matrix.size(); }

	void addEdge(size_t src, size_t dest) {
		matrix[src].push_back(dest);
		matrix[dest].push_back(src);
	}
	const std::vector<size_t>& adjacentNodesOf(size_t node) const {
		return matrix[node];
	}
};