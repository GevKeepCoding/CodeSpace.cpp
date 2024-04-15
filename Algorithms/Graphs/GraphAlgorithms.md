
# Graph Algorithms
- [X] BFS
- [X] DFS
- [X] Dijkstra's Algorithm
- [] Bellman-Ford Algorithm
- [] Floyd-Warshall Algorithm
- [] Prim's Algorithm
- [] Kruskal's Algorithm
- [] Topological Sort
	- [] Kahn's Algorithm
	- [] Tarjan's Algorithm
	- [] Depth-First Search
	- [] Breadth-First Search?
	- [] Lexicographical Topological Sort?

---
- [ ] Strongly Connected Components
	- [ ] [Kosaraju's Algorithm for Strongly Connected Components](https://www.youtube.com/watch?v=HOOmetF56BI)
	- [ ] [Strongly Connected Components Kosaraju's Algorithm Graph Algorithm](https://www.youtube.com/watch?v=RpgcYiky7uw)
	- [ ] 
---
- [ ] Minimum Spanning Tree
	- [ ] Prim's Algorithm
	- [ ] Kruskal's Algorithm
	- [ ] Boruvka's Algorithm
	- [ ] Jarnik's Algorithm
	- [ ] Reverse-Delete Algorithm
---
- [] Johnson's Algorithm
- [] Ford-Fulkerson Algorithm
- [] A* Algorithm


- [] Edmonds-Karp Algorithm
- [] Hopcroft-Karp Algorithm
- [] Tarjan's Algorithm
- [] Kosaraju's Algorithm
- [] Gabow's Algorithm
- [] Chokker-Vizing Algorithm
- [] Erdos-Gallai Algorithm
- [] Bron-Kerbosch Algorithm
- [] Hungarian Algorithm
- [] Gale-Shapley Algorithm
- [] Dinic's Algorithm
- [] Aho-Corasick Algorithm
- [] Boyer-Moore Algorithm
- [] Rabin-Karp Algorithm
- [] Knuth-Morris-Pratt Algorithm
- [] Z Algorithm
- [] Manacher's Algorithm
- [] Suffix Array
- [] Suffix Tree
- [] Suffix Automaton
- [] Burrows-Wheeler Transform



---
## BFS
Used to search graph data structure for a node that meets a set of criteria.
BFS can be used for various purposes, such as:
* Finding the shortest path between two nodes
* Finding all nodes within one connected component
* Finding all nodes that are reachable from a given node
* Testing if a graph is bipartite
* Finding the minimum spanning tree of a graph
* Finding a diameter of a tree


Implementation:
* Start by putting any one of the graph's vertices at the back of the queue and mark it as visited.
* Take the front item of the queue and add its unvisited adjacent nodes to the back of the queue and mark them as visited.
* Repeat step 2 until the queue is empty or you find the node you are looking for.

---
## DAG's
Given that you have an **Directed Acyclic Graph**
Then if you want to iterate it's nodes in an order that any already processed
	vertex does not depend from not processed ones.
Then you should first create **Post-ordered DFS** output and iterate on those vertexes.
