# Asignment 1
This covers the basic inroductory of parrellel programming and the concept be the systems which they run on.

### Table of Contents
1. [Contributors](#Contributors)
2. Question 1
3. Question 2
4. Question 3
5. Question 4
6. Question 5
7. Code Files
8. Attributions

## Contributors
**Student** | **ID**
:---: | ---
Christopher McArthur | `40004257`

## Question 1
All the tests were done with the same number of darts and rounds.

- A) Code provided in `mpi-reduce.c` and ran with -n == 1 
  - the runtime was 0.246764 (i'm assuming seconds)

- B) `mpi-reduce.c` and ran with varrying -n

**number of workers** | **time (in s)**
:---: | ---
2 | 0.260926
5 | 0.275430
10 | 0.325272
25 | 0.352624
100 | 0.882547
1000 | 5.211698

- C) This is a fine grain solution because there is a proccess created for each task. This is an SPDM parrelel program.

- D) `mpi-parent.c` with -n == 1 and  `mpi-child.c` spawn with an internal variable changed in the parent code for number of children.

**number of workers** | **time in s**
:---: | ---
2 | 1.304495
5 | 1.545524
10 | 1.949735
25 | 2.461107
100 | 3.053408
1000 | 29.674014

- E)
  - From A) This speed up graph has both the X and Y asymptotes, its a nice rational function f(x) = 1/x. The over head from communication explains the slowdown seen with fewer number of processes but increasing the number of workers to the maxium of 1000 the time to create all the processes on the server kills the runtime.
![image](https://user-images.githubusercontent.com/16867443/31311499-5aecbcf0-ab7b-11e7-9fed-8c41ca0c23e0.png)

  - From D) This speed up graph has both the X and Y asymptotes, its a nice rational function f(x) = 1/x. The over head from communication exponentially grows with the number of children as the bottleneck for the execution time; this explains the slow down seen.
![image](https://user-images.githubusercontent.com/16867443/31311712-0499da36-ab80-11e7-8e8a-26c701af1ad6.png)

## Question 2
This is a pseudo-c++ object oriented approach to solve the Pi convergence algorithm. I'm a native C++ developer with hardcore OOP tendencies.
```c++
function Master::GetWorkerFromPool(double darts, double rounds, CommSummer* sum)
{
    Worker* worker = m_WorkerPool.GetFree()
    if(worker is not nullptr) returne
    for each (rounds)
       for each(darts)
          sum->SumQueue(worker->pi_calc())
}

function CalcPi()
{
   while(Master.PiDiff() +/- (PI_VALUE - 0.00001) is not true)   
      Master.GetWorkerFromPool( Master.PiDiff() * 333.333333,   Master.PiDiff().floored() * 157.357892514,  this)
}
```
## Question 3
- A) with at least 10 CPUs the best best parrellel time is 9 units of time... this puts the efficiency at aprox. 0.1915.
- B) decrease is the only option which could improve the efficiency. This is because effiency is calculated by speedup divided by the number of processors, the lower the number of CPUs the larger efficiency is likely to be.
- C) The runtime with 4 identical CPUs is 16t (see table below); Making the speedup = 0.3404.

**CPU number** | **Cycle One** | **Cycle Two** | **Cycle Three** |**Cycle Four** | **Cycle Five** | **Cycle Six**
--- | --- | --- | --- | --- | --- | ---
0 |  x  | 2t | 2t | 2t | 5t | x
1 |  x  | 2t | 2t | 2t | 5t | x
2 | *1t* | *2t* | *2t* | *5t* | *5t* | *1t*
3 |  x  | 2t | 2t | 5t | 5t | x

- D) CPUs 0 and 1 are the faster ones, CPUs 2 and 3 are the slower. The table cells with postfixed pluses followed by prefixed pluses. The speed up for this solution is 0.4893 at 23t.

**CPU number** | **Cycle One** | **Cycle Two** | **Cycle Three** |**Cycle Four** | **Cycle Five** | **Cycle Six** | **Cycle Seven** | **Cycle Eight**
--- | --- | --- | --- | --- | --- | --- | --- | ---
0 |  *1t*  | *2t* | *2t* | *2t* | *5t* | 5t | x | *1t*
1 |  x  | 2t | 2t | 2t | 5t | 5t | x | x
2 |  x  | 2t + | + 2t | 2t + | + 2t | *5t* | *5t* | x
3 |  x  | 2t + | + 2t | 2t + | + 2t | x | x | x

## Question 4

## Question 5

### Attributions
* Q1-A: (https://stackoverflow.com/a/5301414)
* Q1-D: (http://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-2.0/node98.htm)
