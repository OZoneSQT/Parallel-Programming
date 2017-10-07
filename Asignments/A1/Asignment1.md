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

## Question 2
pseudo code for doing this dynamically

## Question 3
- A)
- B)
- C)

## Question 4

## Question 5

### Attributions
* Q1-A: (https://stackoverflow.com/a/5301414)
* Q1-D: (http://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-2.0/node98.htm)
