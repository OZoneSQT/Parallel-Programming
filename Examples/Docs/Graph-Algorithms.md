## Graph Algorithms

#### Minimum Spanning Tree
A spanning tree of an undirected graph G is a subgraph of G that is a tree containing all the vertices of G. Prim’s algorithm is a greedy; Start by selecting an arbitrary vertex, include it into the current MST. Grow the current MST by inserting into it the vertex closest to one of the vertices already in current MST. The algorithm works in n outer iterations, the inner loop is relatively easy to parallelize. The adjacency matrix is partitioned in a 1-D block fashion, with distance vector d partitioned accordingly. In each step, a processor selects the locally closest node. followed by a global reduction to select globally closest node which is then inserted into the MST and the choice is broadcasted to everyone. The cost to select the minimum entry is O(n/p + log p) making T<sub>p</sub> = O(n<sup>2</sup>/p + n log p).

#### Single-Source Shortest Paths
Dijkstra’s algorithm is similar to Prim’s algorithm. It maintains a set of nodes for which the shortest paths are known. It grows this set based on the node closest to source using one of the nodes in the current shortest path set. The weighted adjacency matrix is partitioned using the 1-D block mapping. Each process selects, locally, the node closest to the source, followed by a global reduction to select next node The node is broadcast to all processors and the l-vector updated. T<sub>p</sub> = O(n<sup>2</sup>/p + n log p).

#### All-Pairs Shortest Paths
Floyd’s Algorithm using 2-D Block Mapping starts with Matrix D(k) is divided into p blocks of size (n/√p) x (n/√p), each CPU is responsible for its block.

**symbol** | **meaning**
---|---
k | iteration number
D<sup>(k)</sup> | distance matrix at k iteraton
d<sup>(k)</sup><sub>i,j</sub> | distance at k iteration between verticies i and j

To compute d<sup>(k)</sup><sub>i,j</sub> processor P<sub>i,j</sub> must get d<sup>(k-1)</sup><sub>i,k</sub> and d<sup>(k-1)</sup><sub>k,j</sub>

![image](https://user-images.githubusercontent.com/16867443/33693802-d0c0b29e-dac2-11e7-9ad4-47cafdcd9a9f.png)

Once d<sup>(k)</sup><sub>i,j</sub> is calculated it is sent in a one-to-all broadcast along their rows/columns. It begins with the matrix D<sup>(0)</sup> which is the adjacency matrix. The parallel run time of the 2-D block mapping formulation of Floyd’s algorithm is T<sub>p</sub> = &#x3f4;(n<sup>3</sup>/p) + &#x3f4;(n<sup>2</sup>√/p log p).Is cost optimal when p = O(n<sup>2</sup> / log<sup>2</sup> n) and the isoefficiency of this formulation is &#x3f4;(p<sup>1.5</sup>log<sup>2</sup> p). This can be improved by relaxing the sychrnozation of all processes waiting for the required elements.
