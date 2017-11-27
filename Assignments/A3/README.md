# Assignment 3
This assignment was about around search parellel algorithms.

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
Floyd's Pairs shortest path.

### Sequential
**Input Size** | **Time (s)**
---|---
4 | 0.00129795
16 | 0.000683069
32 | 0.000838041
64 | 0.000921965

Parallel version never finish =/

## Question 2
#### Disadvantages
* With a large set of processors all starting at root, this may take time to access the node concurently and it may take time for each processor to have a node
* A poorly balanced tree will yield a lot of wasted processing time
* If the goal node is a leaf it may take a while before it is reached with larger data sets

#### Advantages
*	In the case of a well balanced tree, processors will be well used and busy

## Question 3
## Question 4

## Attributions
* https://github.com/hownowstephen/concordia-comp428-3
