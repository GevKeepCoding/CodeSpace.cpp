#pragma once
import std;
using namespace std;

//Acing Google Coding Interview as an 18 year old High School Student https://www.youtube.com/watch?v=-tNMxwWSN_M
class google_coding_interview_1 {


	struct task1 {
		// find sum of all depts in the binary tree.

		struct node
		{
			node* left;
			node* right;
			int value;
		};

		int ans{0};

		void dfs(node* u, int depth) {
			ans += depth;
			if (u->left)
				dfs(u->left, depth + 1);
			if (u->right)
				dfs(u->right, depth + 1);
		}


		int sumAllDepths(node* root) {
			ans = 0;
			dfs(root, 0);
			return ans;
		}
	};


	//Acing Google Coding Interview as an 18 year old High School Student https://www.youtube.com/watch?v=-tNMxwWSN_M
	struct task2 {

		// find sum of all depts in the binary tree.

		struct node
		{
			node* left;
			node* right;
			int value;
		};

		int ans{0};

		void dfs(node* u, int depth) {
			ans += depth;
			if (u->left)
				dfs(u->left, depth + 1);
			if (u->right)
				dfs(u->right, depth + 1);
		}


		int sumAllDepths(node* root) {
			ans = 0;
			dfs(root, 0);
			return ans;
		}
	};


	//Acing Google Coding Interview as an 18 year old High School Student
	struct task3 {
	// find sum of all depts in the binary tree and it's subtrees.

		struct node
		{
			node* left;
			node* right;
			int value;
		};

		int ans{0};

		void dfs1(node* u, int depth) {
			ans += depth;
			if (u->left)
				dfs1(u->left, depth + 1);
			if (u->right)
				dfs1(u->right, depth + 1);
		}


		int dfs2(node* u) {
			if (u->left)
			{
				dfs1(u->left, 1);
				dfs2(u->left);
			}
			if (u->right)
			{
				dfs1(u->right, 1);
				dfs2(u->right);
			}
		}



		int sumAllDepths(node* root) {
			ans = 0;
			dfs2(root);
			return ans;
		}
	};

	// sum of distances of all nodes from target
	class task4
	{
		struct node
		{
			node* left;
			node* right;
			int value;
			int subTreeSize{0};
		};

		int calcSubTreeSizes(node* root) {
			if (!root) return 0;
			root->subTreeSize = 1 + calcSubTreeSizes(root->left) + calcSubTreeSizes(root->right);
			return root->subTreeSize;
		}

		int sumDistFrom_Target{0};

		int sumDist(node* root) {
			int ans = 0;
			if (root->left)
				ans += sumDist(root->left);
			if (root->right)
				ans += sumDist(root->right);
			return ans;
		}

		void dfs(node* root, node* target, int sumDistRoot) {
			if (root == target)
				return;
			if (root->left) {
				int root_left_dist = sumDistRoot - root->left->subTreeSize + (root->subTreeSize - root->left->subTreeSize);
				if (root->left == target)
				{
					sumDistFrom_Target = root_left_dist;
					return;
				}
				else
					dfs(root->left, target, root_left_dist);
			}
			if (root->right) {
				int root_right_dist = sumDistRoot - root->right->subTreeSize + (root->subTreeSize - root->right->subTreeSize);
				if (root->right == target)
				{
					sumDistFrom_Target = root_right_dist;
					return;
				}
				else
					dfs(root->right, target, root_right_dist);
			}
		}


		int rootDist{0};
		int sumDistFromTarget(node* root, node* target) {
			calcSubTreeSizes(root);
			sumDistFrom_Target = 0;

			if (root == target)
				return sumDist(root);

			rootDist = sumDist(root);

			dfs(root, target, rootDist);
			// sumDist(2) = SumDist(1) - (number of nodes in subtree 2) + (number of nodes outside subtree 2)
			return sumDistFrom_Target;
		}


	};

};

namespace LeetCode
{
class SolutionMinFuelCost {
public:
	vector<vector<int>> graph;
	vector<bool> visited;
	int res{0};
	int dfs(int root, int seats, int& fuel) {
		int people{1};//root itself
		visited[root] = true;
		for (auto n : graph[root]) {
			if (visited[n] == false) {
				visited[n] = true;
				auto p = dfs(n, seats, fuel);
				auto f = (p + seats - 1) / seats;
				fuel += f;
				people += p;
			}

		}
		//fuel += (people + seats - 1) / seats;
		return people;
	}

	long long minimumFuelCost(vector<vector<int>>& roads, int seats) {
		graph.resize(roads.size() + 1);
		visited.resize(roads.size() + 1, false);
		for (auto& edge : roads) {
			graph[edge[0]].push_back(edge[1]);
			graph[edge[1]].push_back(edge[0]);
		}
		int fuel = 0;
		(void)dfs(0, seats, fuel);
		return fuel;
	}
};

// testing
int main_SolutionMinFuelCost() {
//    roads = [[0, 1], [0, 2], [0, 3]]
	vector<vector<int>> roads = {{0,1}, {0,2}, {0,3}};
	SolutionMinFuelCost s;
	auto res = s.minimumFuelCost(roads, 5);
}
}

namespace binary_tree_traversals
{
struct node {
	int val;
	node* left = nullptr;
	node* right = nullptr;
	
	node(int val) : val(val) {}
};
// left -> right -> root
vector<node*> postorder_iterative_traversal(node* root)
{
	vector<node*> result;
	stack<node*> s;

	if (root == nullptr) return result;

	s.push(root);
	while (!s.empty())
	{
		root = s.top(); s.pop();
		result.push_back(root);

		if (root->left) s.push(root->left);
		if (root->right) s.push(root->right);
	}
	reverse(result.begin(), result.end());

	return result;
}

// Iterative PostOrder traversal with one stack:
vector<node*> postorder_iterative_traversal_oneStack(node* root)
{
	vector<node*> res;
	stack<node*> s;
	if (root == nullptr) return res;

	s.push(root);
	while (root || !s.empty())
	{
		if (root)
		{
			if (root->left) s.push(root->left);
			root = root->left;
		}
		else
		{
			root = s.top();
			if (root->right) s.push(root->right);
			else
			{
				res.push_back(root);
				s.pop();
				root = nullptr;
			}
		}
	}
}




// root -> left -> right
vector<node*> preorder_iterative_traversal(node* root)
{
	vector<node*> result;
	if (root == nullptr) return result;

	stack<node*> s;
	s.push(root);

	while (!s.empty())
	{
		root = s.top(); s.pop();
		result.push_back(root);

		if (root->right) s.push(root->right);
		if (root->left) s.push(root->left);
	}

	return result;
}
//Iterative Inorder Traversal of Binary Tree: Tushar Roy
// left -> root -> right
vector<node*> inorder_iterative_traversal(node* root)
{
	vector<node*> res;
	if (!root) return res;

	stack<node*> s;
	s.push(root);

	while (!s.empty())
	{
		root = s.top();
		if (root->left)
		{
			s.push(root);
			root = root->left;
		}
		else
		{
			s.pop();
			res.push_back(root);
			if (root->right)
				s.push(root->right);
		}
	}


	return res;
}

// BFS means - level order traversal
void Breadth_first_Search(node* root, auto fun) { // fun can be min, max, sum, etc.
	vector<node*> queue;
	queue.push_back(root);

	while (!queue.empty())
	{
		auto size = queue.size();
		vector<int> level;
		for (int i = 0; i < size; i++)
		{
			auto current = queue.front(); queue.pop_front();
			level.push_back(current->val);
			if (current->left) queue.push_back(current->left);
			if (current->right) queue.push_back(current->right);
		}
		fun(level);
	}
}


}

