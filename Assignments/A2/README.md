# Assignment 2
This assignment is to cover parellel sorting algorithms by the specific implementation of quicksort on a hycube system.

### Table of Contents
1. [Contributors](#contributors)
2. [Question 1](#question-1)
3. [Question 2](#question-2)

## Contributors
**Student** | **ID**
:---:|---
Christopher McArthur | `40004257`

## Question 1
The sequential MPI implementation of quicksort produced a fastest runtime of `0.000078` seconds for an array size of 150, alternating between 3 runtime values.

The hypercube implementation of a parellel quicksort was far less reliable; there was a clear flaw (not every `mpirun` would terminate successfully) in the logic of the communication sequences which I was unable to uncover. It is very likely that this skewed my results, but the runtimes are provided below (selected on the criteria the output was more correct)

**number of workers** | **time in s**
:---: | ---
4 | 6.486495
16 | 2.741684
36 | 2.457224
64 | `INIF`

### Speedup Graph
![image](https://user-images.githubusercontent.com/16867443/32418545-0ecfab5e-c23a-11e7-8920-6ca4769d9b53.png)

## Question 2

