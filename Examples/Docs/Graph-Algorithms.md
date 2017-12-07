## Graph Algorithms

#### Minimum Spanning Tree
A spanning tree of an undirected graph G is a subgraph of G that is a tree containing all the vertices of G. Prim’s algorithm is a greedy; Start by selecting an arbitrary vertex, include it into the current MST. Grow the current MST by inserting into it the vertex closest to one of the vertices already in current MST. The algorithm works in n outer iterations, the inner loop is relatively easy to parallelize. The adjacency matrix is partitioned in a 1-D block fashion, with distance vector d partitioned accordingly. In each step, a processor selects the locally closest node. followed by a global reduction to select globally closest node which is then inserted into the MST and the choice is broadcasted to everyone. The cost to select the minimum entry is O(n/p + log p) making T<sub>p</sub> = O(n<sup>2</sup>/p + n log p).

#### Single-Source Shortest Paths
Dijkstra’s algorithm is similar to Prim’s algorithm. It maintains a set of nodes for which the shortest paths are known. It grows this set based on the node closest to source using one of the nodes in the current shortest path set. The weighted adjacency matrix is partitioned using the 1-D block mapping. Each process selects, locally, the node closest to the source, followed by a global reduction to select next node The node is broadcast to all processors and the l-vector updated. T<sub>p</sub> = O(n<sup>2</sup>/p + n log p).

#### All-Pairs Shortest Paths
###### Floyd’s Algorithm using 2-D Block Mapping
This implementations uses with Matrix D(k) is divided into p blocks of size (n/√p) x (n/√p), each CPU is responsible for its block. It begins with the matrix D<sup>(0)</sup> which is the adjacency matrix of the weighted graph.

**Symbol** | **Meaning**
---|---
k | iteration number
D<sup>(k)</sup> | distance matrix at k iteraton
d<sup>(k)</sup><sub>i,j</sub> | distance at k iteration between verticies i and j

To compute d<sup>(k)</sup><sub>i,j</sub> processor P<sub>i,j</sub> must get d<sup>(k-1)</sup><sub>i,k</sub> and d<sup>(k-1)</sup><sub>k,j</sub> At `k = 0` only the edges are know; as k iterates a new verticies is added the possible path. K is therefore bounded by the number of verticies. Ex...
> `k = 0` above, the only known paths correspond to the single edges in the graph. At `k = 1`, paths that go through the vertex 1 are found: in particular, the path [2,1,3] is found, replacing the path [2,3] which has fewer edges but is longer (in terms of weight). At `k = 2`, paths going through the vertices {1,2} are found. The red and blue boxes show how the path [4,2,1,3] is assembled from the two known paths [4,2] and [2,1,3] encountered in previous iterations, with 2 in the intersection. The path [4,2,3] is not considered, because [2,1,3] is the shortest path encountered so far from 2 to 3. At `k = 3`, paths going through the vertices {1,2,3} are found. Finally, at `k = 4`, all shortest paths are found.

![image](https://user-images.githubusercontent.com/16867443/33693802-d0c0b29e-dac2-11e7-9ad4-47cafdcd9a9f.png)

Once d<sup>(k)</sup><sub>i,j</sub> is calculated it is sent in a one-to-all broadcast along their rows/columns.  The parallel run time of the 2-D block mapping formulation of Floyd’s algorithm is T<sub>p</sub> = &#x3f4;(n<sup>3</sup>/p) + &#x3f4;(n<sup>2</sup>√/p log p).Is cost optimal when p = O(n<sup>2</sup> / log<sup>2</sup> n) and the isoefficiency of this formulation is &#x3f4;(p<sup>1.5</sup>log<sup>2</sup> p). This can be improved by relaxing the sychrnozation of all processes waiting for the required elements.

###### Floyd’s Algorithm using Pipelining
This implementation focuses on removing the synchronization of the previous algorithm. A process begin working on the iteration as soon as the previous is completed. The conceptual difference is that some of the CPUs have already started on the k+1 iteration and are just waiting for the values they neeed to do their computation. Once they have completed thier computation they pass the value to the neightborings CPUs, as the complete thier results the collective results is passed along. T<sub>p</sub> = &#x3f4;(n<sup>3</sup>/p) + &#x3f4;(n) which overall is not very different then the 2D variation but the isometric function is &#x3f4;(p<sup>1.5</sup>) meaning it is more scalable.
