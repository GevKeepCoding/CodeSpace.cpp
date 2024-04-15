## Data Structures









 - [Indexed Priority Queue (UPDATED) | Data Structures](https://www.youtube.com/watch?v=jND_WJ8r7FE)
 
 An Indexed Priority Queue is a
 traditional priority queue variant
 which on top of the regular PQ
 operations supports **quick updates an deletions of key-value pairs**;

Used in Dijkstra's algorithm, Prim's algorithm, A* search algorithm, etc.


In Dijkstra's algorithm, we need to update the distance of a vertex in the priority queue when we find a shorter path to it.
This is where the indexed priority queue comes in handy.

We need a Map to store the index of each vertex in the priority queue.
This way, we can quickly update the distance of a vertex in the priority queue.

{
NodeId: KeyIndex
"Anna" : 0,
'b' : 1,
'c' : 2,
'd' : 3,
'e' : 4,
'f' : 5,
...
}


KeyIndex indexes:    0, 1,  2,  3,  4, 5,  6, 7, 8, 9,  10, 11, 12, 13, 14 
vals:				 3, 15, 11, 17, 7, 9,  2, 1, 6, 5,  16, 4,  -1, -1, -1 
pm:			 	     2, 9,  11, 7,  8, 6,  1, 0, 3, 4,  10, 5,  -1, -1, -1
InverseMap:			 7, 7,  0,  8,  9, 11, 5, 3, 4, 1,  10, 2,  -1, -1, -1


'a' -> 0 -> 2 (priority heap)



(ki = 0, v = 3)  -> in our priority heap it will have index 2 -> 0 (in inverse map) -> "Anna"

---

- [] Tarjan's Strongly Connected Component (SCC) Algorithm (UPDATED) | Graph Theory
- [] Kosaraju's Algorithm for Strongly Connected Components












### Other Data Structures and their Time Complexities.
* **Binary Heap** - O(logN) time complexity for insertions and deletions
* **Fibonacci Heap** - O(1) time complexity for insertions and deletions
* **Van Emde Boas Tree** - O(loglogN) time complexity for insertions and deletions
