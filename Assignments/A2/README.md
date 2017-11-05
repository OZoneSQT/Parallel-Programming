# Assignment 2
This assignment is to cover parellel sorting algorithms by the specific implementation of quicksort on a hycube system.

### Table of Contents
1. [Contributors](#contributors)
2. [Question 1](#question-1)
3. [Question 2](#question-2)
4. [Question 3](#question-3)
5. [Question 4](#question-4)

## Contributors
**Student** | **ID**
:---:|---
Christopher McArthur | `40004257`

## Question 1
The sequential MPI implementation of quicksort produced a fastest runtime of `0.000078` seconds for an array size of 150, alternating between 3 runtime values.

The hypercube implementation of a parellel quicksort was far less reliable; there was a clear flaw (not every `mpirun` would terminate successfully) in the logic of the communication sequences which I was unable to uncover. It is very likely that this skewed my results, but the runtimes are provided below (selected on the criteria the output was more correct)

**number of workers** | **time in s**
:---: | ---
2 | 0.355485
4 | 0.236495
8 | 0.205894
16 | 0.191684
36 | `INIF`

### Speedup Graph
![image](https://user-images.githubusercontent.com/16867443/32419420-af04a61c-c247-11e7-91de-ed5a84afe48a.png)

## Question 2
Assuming pivots are picked producing balanced nodes, sequential time is O(n log(n)). Parellel time requies log(p) itterations of n/p communications followed by quick sorting n/p numbers this puts the theoretical time at O((n/p)log(n/p)) + O((n/p)log(p)). This will make speedup O(n log(n)) / (O((n/p)log(n/p)) + O((n/p)log(p))) and efficency will be O(n log(n)) / p(O((n/p)log(n/p)) + O((n/p)log(p))).

## Question 3
- (i) Load balancing can be equal poor in both algorithms. This is because the quick selection of a pivot can not garauntee a balanced load being passed  to each procesor.
- (ii) Efficency will be a lot worse in the naive implementation because there will be far greater idling. For example with an input size of 1024 and a cut off size of 8; the first processor will quicksort 512 elements and wait for 7 other processors to complete with the additional over head of elements needing to be communicated back and forth while blocking.

## Question 4
Network bitonic sort's runtime is O(n log^2(n)) which may be worse than that of the hypercube quicksort implementation but it does feature better load balancing and efficiency. At all takes of a bitonic sort each processor has the same number of elements and does the same compare-exchange. Since all processors at all takes have the same number of elements the efficiency is better because the hypercube quick sort needs to be balanced with `n = p * k` where K is an interger greater than 0 for this to occur.
