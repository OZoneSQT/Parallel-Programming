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

### Two Dimensional
**Input Size** | **Number of Processors** | **Time (s)**
---|---|---
4 | 2 | 0.000591993
4 | 4 | 0.000446796
4 | 16 | 0.00119281
4 | 32 | 0.00324893
16 | 2 | 0.014143
16 | 4 | 0.000753164
16 | 16 | 0.000710964
16 | 32 | 0.00353599
32 | 2 | 0.00171089
32 | 4 | 0.00758886
32 | 16 | 0.00136709
32 | 32 | 0.043062
64 | 2 | 0.010658
64 | 4 | 0.000442982
64 | 16 | 0.00523305
64 | 32 | 0.00272584

### Pipelined
Parallel Pipelined version was never finish =/

### Efficency Graphs
![image](https://user-images.githubusercontent.com/16867443/33292601-da01f200-d397-11e7-8fc2-f280d6cdde00.png)
![image](https://user-images.githubusercontent.com/16867443/33292751-5378f106-d398-11e7-9d69-4c35b6cb1d30.png)
![image](https://user-images.githubusercontent.com/16867443/33292873-c588b326-d398-11e7-96ae-b6a6894f4862.png)
![image](https://user-images.githubusercontent.com/16867443/33293024-4a17fc0a-d399-11e7-8a1f-051f5fafa44b.png)

### Results
The 2D implementation is far better than the pipeline because it is easier to implement. As seen in the graphs above the efficency tends to decrease as the number of proccessors increases with the exception of `n = 16` and `n = 64` where both showed abnormal values when proccessors was equal to 4. This odd behavoir might be explained by the fact Processors squared is a multiple of the data set and the algorithm is obtains its must efficency.

## Question 2
#### Disadvantages
* With a large set of processors all starting at root, this may take time to access the node concurently and it may take time for each processor to have a node
* A poorly balanced tree will yield a lot of wasted processing time
* If the goal node is a leaf it may take a while before it is reached with larger data sets

#### Advantages
*	In the case of a well balanced tree, processors will be well used and busy

## Question 3
Assume that there is a case that the initiator processor can receive a white token without all processes being terminated.
This implies that the last processor in the ring will pass a white token. Tokens are passed in two ways, either the token is passed as-is, or it converts to black if the process is black. Since we know that the token originates as white from the initiator processor, then if at any point the token encounters a non-idle processor, it will be turned black, and cannot be turned white again. So either the token will encounter only white processors, meaning the program is complete, or the token will arrive black. This implies therefor that it is impossible to receive a white token unless the processes have all successfully terminated, proving this algorithm by contradiction.

## Question 4

## Attributions
* https://github.com/hownowstephen/concordia-comp428-3
