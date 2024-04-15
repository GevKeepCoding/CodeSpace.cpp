#pragma once
import std;
using namespace std;

class BinaryIndexedTree {
public:  BinaryIndexedTree(int size) :
    size(size) {
    tree.resize(size + 1);
}
      void update(int index, int value)
      {
          while (index <= size)
          {
              tree[index] = max(tree[index], value);
              index += index & -index;
          }
      }

      int query(int index)
      {
          int sum = 0;
          while (index > 0)
          {
              sum = max(tree[index], sum);
              index -= index & -index;
          }
          return sum;
      }
private:
    int size;
    vector<int> tree;
};

class Solutionv2 {
public:

    int bestTeamScore(vector<int>& scores, vector<int>& ages) {
        int n = ages.size();
        vector<pair<int, int>> players(n);
        for (int i = 0; i < n; ++i) {
            players[i] = {scores[i], ages[i]};
        }
        sort(players.begin(), players.end());

        int maxAge = *max_element(ages.begin(), ages.end());
        BinaryIndexedTree tree(maxAge);
        for (auto& [score, age] : players)
        {
            tree.update(age, score + tree.query(age));
        }
        return tree.query(maxAge);
    }
};
// testing
int main() {
   /* vector<int> scores = {1,3,7,3,2,4,10,7,5};
    vector<int> ages =   {4,5,2,1,1,2,4,1,4};

    Solution s;
    auto res = s.bestTeamScore(scores, ages);*/

    for (int i = 1; i < 10; i++)
    {
        int b = -i;
        int a = i & b;
        int c = i + a;

        cout << c;
    }
}