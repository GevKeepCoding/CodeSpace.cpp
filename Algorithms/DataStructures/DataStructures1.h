#pragma once

import std;
using namespace std;


// Disjoint Set (Union Find) Data Structure : https://www.youtube.com/watch?v=PGZ64ob440I
namespace smart_trees
{
	// given that we have array with n numbers let's allocate n+1 array with -1's (to start index from 1).


class union_find
{
	std::vector<int> a; // n+1 elemnts, starts at index 1
	size_t maxSetIdx{0};
public:

	union_find(int n) : a(n + 1, -1) {}

	void Union(int i, int k) {
		int ip = Find(i);
		int kp = Find(k);

		if (ip == kp) return;// already in the same set

		int iSize = a[ip];
		int kSize = a[kp];

		// merge smaller set into the big one
		if (iSize <= kSize) {
			// we are actually "increasing" the size of that set.
			// It's just that, the size is stored as a negative number.
			a[ip] += kSize;
			a[kp] = ip;
		}
		else {
			a[kp] += iSize;
			a[ip] = kp;
		}
	}

	void UnionAndFindLargestSetSize(int i, int k) {
		int ip = Find(i);
		int kp = Find(k);

		if (ip == kp) return;// already in the same set

		int iSize = a[ip];
		int kSize = a[kp];

		// merge smaller set into the big one
		if (iSize <= kSize) {
			// we are actually "increasing" the size of that set.
			// It's just that, the size is stored as a negative number.
			a[ip] += kSize;
			a[kp] = ip;
			if (-a[maxSetIdx] < -a[ip])
				maxSetIdx = ip;
		}
		else {
			a[kp] += iSize;
			a[ip] = kp;
			if (-a[maxSetIdx] < -a[kp])
				maxSetIdx = kp;
		}
	}

	int Find(int i) {
		std::vector<int> paths;
		while (a[i] > 0) {
			paths.push_back(i);
			i = a[i]; // walk up to the parent
		}

		// path compression - point all directly to root
		for (int path : paths) {
			a[path] = i;
		}
		return i;
	}// I guess this is slow we can do much better.
	// Find has amortized constant time complexity. 
};
}




// Fenwick Tree (Binary Indexed Tree)
namespace smart_trees
{
	// https://www.geeksforgeeks.org/binary-indexed-tree-or-fenwick-tree-2/
	// https://www.topcoder.com/thrive/articles/Binary%20Indexed%20Trees%20Tutorial
	/*
		Example problem:
		Suppose we have an array of n elements. We need to support two operations:
		1. Update the value of an element at a given index.
		2. Find the total sum of the elements in the array from index i to j.

	*/
	// Fenwick Tree(Binary Index Tree) - Quick Tutorial and Source Code Explanation : https://www.youtube.com/watch?v=uSFzHCZ4E-8
class fenwick_tree {

	std::vector<int> fw;
public:
	/*
	 Slow n(log(n)) construction
	fenwick_tree(const vector<int>& arr) {
		vector<int> fw(arr.size() + 1, 0);
		for (int i = 0; i < arr.size(); ++i) {
			for (int j = i + 1; j < fw.size(); j += (j & -j)) {
				fw[j] += arr[i];
			}
		}
		return fw;
	}*/


	fenwick_tree(const vector<int>& arr) : fw(arr) { // suppose that array we are given is 1-based(it's possible if we add 0 at the beginning for example).

		for (int i = 1; i < fw.size(); ++i) {
			int j = i + (i & -i);
			if (j < fw.size())
				fw[j] += fw[i];
		}
	}// O(n) construction

	void update(int i, int k) {
		while (i < fw.size()) {
			fw[i] += k;
			i += i & -i;// add last set bit
		}
	}// log(N) update

	int sum(int i) {
		int sum = 0;
		for (; i > 0; i -= (i & -i))
			sum += fw[i];
		return sum;
	}// log(N) sum

}; // we can use +, * , xor, min, max, gcd, lcm, etc. as operation in fenwick tree
// insert and delete operations are not supported in Fenwick tree (they are expensive)

void test_fw_tree1() {
	vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8};
	auto fw = smart_trees::fenwick_tree(arr);

}

namespace Fenwick_tree_usage_example
{
struct BITree {
	vector<int> elems;
	size_t n;
	BITree(size_t n) : n(n), elems(n + 1, 0) {};

	void update(int i, int value) {
		for (; i < n + 1; i += (i & (-i)))
			elems[i] = max(elems[i], value);
	}

	int query(int i) {
		int max_score = 0;
		for (; i > 0; i -= (i & (-i)))
			max_score = max(max_score, elems[i]);
		return max_score;
	}
};

class Solution {
public:
	int bestTeamScore(vector<int>& scores, vector<int>& ages) {
		vector<pair<int, int>> elems;
		for (int i = 0; i < scores.size(); ++i)
			elems.emplace_back(scores[i], ages[i]);
		sort(begin(elems), end(elems));

		int ans = 0;

		auto bit = BITree(*max_element(ages.begin(), ages.end()));

		for (const auto [score, age] : elems) {
			const int curr_best = score + bit.query(age);
			bit.update(age, curr_best);
			ans = max(ans, curr_best);
		}
		return ans;
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

class kd_tree
{
	const int K = 2;
	struct Point {
		int x;
		int y;
	
		int operator[](int idx) const {
			return idx == 0 ? x : y;
		}	
	};
	struct node {
		Point point;
		node* left;
		node* right;
		node(Point p) : point(p), left(nullptr), right(nullptr) {}
	};

	long distSquared(Point a, Point b) {
		return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
	}

	node* closest(Point target, node* a, node* b) {
		if (a == nullptr) return b;
		if (b == nullptr) return a;

		long d1 = distSquared(target, a->point);
		long d2 = distSquared(target, b->point);
		return d1 < d2 ? a : b;
	}

	node* nearestNeighbor(node* root, Point target, int depth)
	{
		if (root == nullptr) return nullptr;

		node* nextBranch, * otherBranch;
		if (target[depth % K] < root->point[depth % K]) {
			nextBranch = root->left;
			otherBranch = root->right;
		}
		else
		{
			nextBranch = root->right;
			otherBranch = root->left;
		}

		node* temp = nearestNeighbor(nextBranch, target, depth + 1);
		node* best = closest(target, temp, root);

		long radiousSquared = distSquared(target, best->point);
		long dist = target[depth % K] - root->point[depth % K];
		if (radiousSquared >= dist * dist) {
			temp = nearestNeighbor(otherBranch, target, depth + 1);
			best = closest(target, temp, best);
		}
		return best;
	}

};




}




// Segment Tree Data Structure - Min Max Queries - Java source code: https://www.youtube.com/watch?v=xztU7lmDLv8
// (Used for finding min/max in a range etc.)
// uses more memory than Fenwick Tree
namespace smart_trees
{

class segment_tree
{
	vector<int> tree;
	// 0th index can be ignored in a tree.
	int n;
public:
	segment_tree(const vector<int>& arr) : tree(arr.size() * 2, 0) {
		std::copy(arr.begin(), arr.end(), tree.begin() + arr.size());
		n = int(arr.size());

		for (size_t i = n - 1; i > 0; i--) {
			tree[i] = std::max(tree[2 * i], tree[2 * i + 1]);
		}
	}

	void update(int i, int value) {
		i += n;
		tree[i] = value;
		int newValue;

		while (i > 1)
		{
			i /= 2;
			newValue = std::max(tree[2 * i], tree[2 * i + 1]);
			if (newValue == tree[i]) break;
			tree[i] = newValue;
		}
	}// O(logn) update

	int query(int l, int r) {
		int res = numeric_limits<int>::min();
		for (l += n, r += n; l < r; l /= 2, r /= 2) {
			if (l % 2) res = std::max(res, tree[l++]);
			if (r % 2) res = std::max(res, tree[--r]);
		}
		return res;
	}// O(logn) query
	// l is inclusive, r is exclusive
}; // we can use +, * , xor, min, max, gcd, lcm, etc. as operation in segment tree


void test_segment_tree() {
	vector arr = {1,2,3};
	auto st = smart_trees::segment_tree(arr);
	cout << st.query(0, 1);

}

}






namespace smart_trees
{
	// AVL Tree
}


namespace smart_trees
{

//todo: learn more use cases for tries.

class TrieNode {
public:
	map<char, TrieNode*> children;
	bool endOfWord;

	TrieNode() : endOfWord(false) {}
};

class Trie {
public:
	TrieNode* root;

	Trie() : root(new TrieNode()) {}

	void insert(const string& word) {
		TrieNode* current = root;
		for (char c : word) {
			if (current->children.count(c) == 0) {
				current->children[c] = new TrieNode();
			}
			current = current->children[c];
		}
		current->endOfWord = true;
	}

	bool search(const string& word) {
		TrieNode* current = root;
		for (char c : word) {
			if (current->children.count(c) == 0) {
				return false;
			}
			current = current->children[c];
		}
		return current->endOfWord;
	}
};

} // namespace smart_trees
