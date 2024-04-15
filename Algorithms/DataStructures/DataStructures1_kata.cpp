import std;
using namespace std;













































































// Disjoint Set (Union Find) Data Structure : https://www.youtube.com/watch?v=PGZ64ob440I
namespace smart_trees
{
	// given that we have array with n numbers let's allocate n+1 array with -1's (to start index from 1).


class union_find
{
	std::vector<int> a; // n+1 elements, starts at index 1
	size_t maxSetIdx{0};
public:

	union_find(int n) : a(n + 1, -1) {}

	void Union(int i, int k) {
		
	}

	void UnionAndFindLargestSetSize(int i, int k) {
		
	}

	int Find(int i) {
		
	}// I guess this is slow we can do much better.
	// Find has amortized constant time complexity. 
};
}

namespace smart_trees
{
	/*
		Example problem:
		Suppose we have an array of n elements. We need to support two operations:
		1. Update the value of an element at a given index.
		2. Find the total sum of the elements in the array from index i to j.
	*/
class fenwick_tree {

	std::vector<int> fw;
public:
	/*
	 Slow n(log(n)) construction
	fenwick_tree(const vector<int>& arr) {
		
	}*/


	fenwick_tree(const vector<int>& arr) : fw(arr) { // suppose that array we are given is 1-based(it's possible if we add 0 at the beginning for example).

		
	}// O(n) construction

	void update(int i, int k) {

	}// log(N) update

	int sum(int i) {
		return 0;
	}// log(N) sum

}; // we can use +, * , xor, min, max, gcd, lcm, etc. as operation in fenwick tree
// insert and delete operations are not supported in Fenwick tree (they are expensive)


namespace Fenwick_tree_usage_example
{
struct BITree {
	vector<int> elems;
	size_t n;
	BITree(size_t n) : n(n), elems(n + 1, 0) {};

	void update(int i, int value) {

	}

	int query(int i) {
		
	}
};

class Solution {
public:
	int bestTeamScore(vector<int>& scores, vector<int>& ages) {
		
	}
};
}
}


// KD-Tree Nearest Neighbor Data Structure : https://www.youtube.com/watch?v=Glp7THUpGow
namespace smart_trees
{
	/*
		// https://www.geeksforgeeks.org/k-dimensional-tree-set-3-delete/

		This tree is used when we have a points in a plane
		and we want to find the nearest point to a given point.
		// Source code: https://bitbucket.org/StableSort/play/src/master/src/com/stablesort/kdtree/KDTree.java

	*/


}




namespace smart_trees
{
	// Segment Tree
}

namespace smart_trees
{
	// AVL Tree
}


