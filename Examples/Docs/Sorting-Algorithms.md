## Sorting Algorithms
The general asumption is that The sorted list is partitioned with the property that each partitioned list is sorted and each element in processor P<sub>*i*</sub>’s list is less than that in P<sub>*j*</sub>’s list if *i* < *j*.

A parallel compare-exchange operation. Processes P<sub>*i*</sub> and P<sub>*j*</sub> send their elements to each other. Process P<sub>*i*</sub> keeps min{a<sub>*i*</sub>, a<sub>*j*</sub>}, and P<sub>*j*</sub> keeps max{a<sub>*i*</sub>, a<sub>*j*</sub>}.

#### Bitonic Sort
A Bitonic sequence is list comprised of two halves, the first half is increasing the second half is decreasing and can start at any element in the given list. When `p = n` and when mapped to a hypercube, each step of the algorithm, every process performs a compare-exchange operation with a neightbor, 

![image](https://user-images.githubusercontent.com/16867443/33642726-3749c57e-da09-11e7-9fd3-71ceed716f19.png)

this takes &#x3f4;(1) making the T<sub>p</sub> = &#x3f4;(log<sup>2</sup> n). This is cost optimal compared to its counter part but not the best algorithm. When running on a mesh with `p = n`,, the compare-exchange takes place with its *i*<sup>th</sup> corresponding pair. This added level of communication brings T<sub>p</sub> = T<sub>comp</sub> + T<sub>comm</sub> == &#x3f4;(log<sup>2</sup> n) + &#x3f4;(n<sup>1/2</sup>) which is no longer cost optimal.

When `p < n`, additional local sorting is needed at each step, with even load balancing throught out local sort takes T<sub>sort</sub> = &#x3f4;((n/p)log (n/p)). the new parallel time is T<sub>p</sub> = T<sub>comp</sub> + T<sub>comm</sub> + T<sub>sort</sub> == &#x3f4;((n/p)log<sup>2</sup> p) + &#x3f4;((n/p)log<sup>2</sup> p) + &#x3f4;((n/p)log (n/p))

![image](https://user-images.githubusercontent.com/16867443/33670665-97f7eff8-da73-11e7-9763-90a301e9071d.png)

#### Odd-Even Transposition
In this sort algorithm, the initial array of size *n* goes through two phases a total of *n* times (half each phase). The even phase pairs all the even index elements with the *i* + 1 element; each of these pairs undergoes a compare and exchange operation. The next phase is the odd where all the odd indexed elements a paired with their *i* + 1 element which performs the operation; this inherently leaves out the first and last element. 

![image](https://user-images.githubusercontent.com/16867443/33680954-2e86c33c-da91-11e7-9440-6a9e5a6a94e1.png)

When `p = n` the T<sub>p</sub> = &#x3f4;(n) this is optimal compared to the serial version but not the best sort. When `p < n` the compare and exchange operation is replaced with a compare and spilt which adds the complexity of a local sort, compairsion of all elements and communication overhead. this brings T<sub>p</sub> = &#x3f4;((n/p)log (n/p)) + &#x3f4;(n) + &#x3f4;(n). This is cost optimal when p = O(log n) and the isometric function is &#x3f4;(p2<sup>p</sup>).

#### Shellsort
This algorithm has two distinct phases, it begins by comparing the outter elements and the inner is the inner elements. once a stage is complete the list is spilt in to and recursively continues. Once the sublists reach a size of one, the second phase begins which is aan odd-even transposition.

![image](https://user-images.githubusercontent.com/16867443/33681454-95a51608-da92-11e7-8ed6-1aa695f1de34.png)

Each CPU does log p compare-split operations, each communication can be performed in time &#x3f4;(n/p), and l odd and even phases are performed making the T<sub>p</sub> = &#x3f4;((n/p)log (n/p)) + &#x3f4;((n/p)log p) + &#x3f4;(l(n/p)).
