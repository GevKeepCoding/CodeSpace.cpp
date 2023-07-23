
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
