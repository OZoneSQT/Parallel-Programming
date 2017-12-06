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

#### Quicksort
Quicksort selects one of the entries in the sequence to be the pivot and divides the sequence into two – one with all elements less than the pivot and other greater. The process is recursively applied to each of the sublists. The performance of quicksort depends critically on the quality of the pivot. In the best case, even splits of the lists, the complexity of quicksort is O(n log n).

###### Shared Address Space
a list of size n equally divided across p processors, A pivot is selected by one of the processors and made known to all processors. Each processor partitions its list into two, say L<sub>*i*</sub> and U<sub>*i*</sub>, based on the selected pivot. All of the L<sub>*i*</sub> lists are merged and all of the U<sub>*i*</sub> lists are merged separately. CPUs are distrubuted to each list maintaining n/p ration and the processes is applied recursively.

The challenge is the global merge of the lists, a technique called prefix sum is used to do this efficiently. Each CPU writes into an array at it's idex the number of elements in has in L<sub>*i*</sub> and U<sub>*i*</sub> in a seperate array. following this it calculates the sum of all the indexes before its own, this gives the prefix sum which corresponds to the index it should being writing to in the global array.

![image](https://user-images.githubusercontent.com/16867443/33687633-ba1f020a-daa6-11e7-8b85-cf4188a2b6e6.png)

First to determine and broadcast the pivot takes &#x3f4;(log p); locally rearrange the array takes &#x3f4;(n/p); determine the locations in the globally rearranged array takes &#x3f4;(log p); and lastly perform the global rearrangement takes &#x3f4;(n/p). Overall this process takes &#x3f4;(log p) + &#x3f4;(n/p). T<sub>p</sub> = &#x3f4;((n/p)log (n/p)) + &#x3f4;((n/p)log p) + &#x3f4;(log<sup>2</sup> p). Ths corresponding isometric function is &#x3f4;(p log<sup>2</sup> p).

###### Message Passing
based on the recursive halving of the machine, each processor in the lower half is paired with a corresponding processor in the upper
half. A designated processor selects and broadcasts the pivot; each processor partitions its list into two, say L<sub>*i*</sub> and U<sub>*i*</sub>, based on the selected pivot. The upper CPU's L<sub>*i*</sub> is sent to the lower who sends its U<sub>*i*</sub> back. The above process is recursed until each processor has its own local list, which is sorted locally. To determine and broadcast the pivot takes &#x3f4;(log p); locally rearrange the array takes &#x3f4;(n/p); for exchanging lists and reorganization takes &#x3f4;(n/p). Tis means T<sub>p</sub> = &#x3f4;((n/p)log (n/p)) + &#x3f4;((n/p)log p) + &#x3f4;(log<sup>2</sup> p). Ths corresponding isometric function is &#x3f4;(p log<sup>2</sup> p) which is the same as the shared address space implementation.
