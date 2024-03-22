#pragma once
//Acing Google Coding Interview as an 18 year old High School Student https://www.youtube.com/watch?v=-tNMxwWSN_M
class google_coding_interview_1 {


	struct task1 {
		// find sum of all depts in the binary tree.

		struct Node
		{
			Node* left;
			Node* right;
			int value;
		};

		int ans{0};

		void dfs(Node* u, int depth) {
			ans += depth;
			if (u->left)
				dfs(u->left, depth + 1);
			if (u->right)
				dfs(u->right, depth + 1);
		}


		int sumAllDepths(Node* root) {
			ans = 0;
			dfs(root, 0);
			return ans;
		}
	};


	//Acing Google Coding Interview as an 18 year old High School Student https://www.youtube.com/watch?v=-tNMxwWSN_M
	struct task2 {

		// find sum of all depts in the binary tree.

		struct Node
		{
			Node* left;
			Node* right;
			int value;
		};

		int ans{0};

		void dfs(Node* u, int depth) {
			ans += depth;
			if (u->left)
				dfs(u->left, depth + 1);
			if (u->right)
				dfs(u->right, depth + 1);
		}


		int sumAllDepths(Node* root) {
			ans = 0;
			dfs(root, 0);
			return ans;
		}
	};


	//Acing Google Coding Interview as an 18 year old High School Student
	struct task3 {
	// find sum of all depts in the binary tree and it's subtrees.

		struct Node
		{
			Node* left;
			Node* right;
			int value;
		};

		int ans{0};

		void dfs1(Node* u, int depth) {
			ans += depth;
			if (u->left)
				dfs1(u->left, depth + 1);
			if (u->right)
				dfs1(u->right, depth + 1);
		}


		int dfs2(Node* u) {
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



		int sumAllDepths(Node* root) {
			ans = 0;
			dfs2(root);
			return ans;
		}
	};

	// sum of distances of all nodes from target
	class task4
	{
		struct Node
		{
			Node* left;
			Node* right;
			int value;
			int subTreeSize{0};
		};

		int calcSubTreeSizes(Node* root) {
			if (!root) return 0;
			root->subTreeSize = 1 + calcSubTreeSizes(root->left) + calcSubTreeSizes(root->right);
			return root->subTreeSize;
		}

		int sumDistFrom_Target{0};

		int sumDist(Node* root) {
			int ans = 0;
			if (root->left)
				ans += sumDist(root->left);
			if (root->right)
				ans += sumDist(root->right);
			return ans;
		}

		void dfs(Node* root, Node* target, int sumDistRoot) {
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
		int sumDistFromTarget(Node* root, Node* target) {
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

