# Asignment 1
This covers the basic inroductory of parrellel programming and the concept be the systems which they run on.

### Table of Contents
1. [Contributors](#Contributors)
2. [Question 1](#Question-1)
3. [Question 2](#Question-2)
4. [Question 3](#Question-3)
5. [Question 4](#Question-4)
6. [Question 5](#Question-5)
7. [Code Files](#Code-Files)
8. [Attributions](#Attributions)

## Contributors
**Student** | **ID**
:---: | ---
Christopher McArthur | `40004257`

## Question 1
All the tests were done with the same number of darts and rounds.

- A) Code provided in [`mpi-reduce.c`](#mpi-reduce.c) and ran with -n == 1 
  - the runtime was 0.246764 (i'm assuming seconds)

- B) [`mpi-reduce.c`](#mpi-reduce.c) and ran with varrying -n. Sample output is included below.

**number of workers** | **time (in s)**
:---: | ---
2 | 0.260926
5 | 0.275430
10 | 0.325272
25 | 0.352624
100 | 0.882547
1000 | 5.211698

- C) This is a fine grain solution because there is a proccess created for each task. This is an SPDM parrelel program.

- D) [`mpi-parent.c`](#mpi-parent.c) with -n == 1 and  [`mpi-child.c`](#mpi-child.c) spawn with an internal variable changed in the parent code for number of children.

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
- A) With at least 10 CPUs the best best parrellel time is 9 units of time... this puts the efficiency at aprox. 0.1915.
- B) Decrease is the only option which could improve the efficiency. This is because effiency is calculated by speedup divided by the number of processors, the lower the number of CPUs the larger efficiency is likely to be.
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
- A) Under flawless conditions, the best parrellaelized time is equal to the depth of the tree whic his log(n).
- B) The sequestial time for his problem is O(log(n)), this puts efficiency at O(1/n)
- C) This solution is far from cost optimal because it's a fine grain attempt to solve the problem but in the best case obtains the runtime for a sequencial system.

## Question 5
I am not proving anything mathematically.

## Code Files

### mpi-reduce.c
```c
/**********************************************************************
 * FILE: mpi_pi_reduce.c
 * OTHER FILES: dboard.c
 * DESCRIPTION:  
 *   MPI pi Calculation Example - C Version 
 *   Collective Communication example:  
 *   This program calculates pi using a "dartboard" algorithm.  See
 *   Fox et al.(1988) Solving Problems on Concurrent Processors, vol.1
 *   page 207.  All processes contribute to the calculation, with the
 *   master averaging the values for pi. This version uses mpc_reduce to 
 *   collect results
 * AUTHOR: Blaise Barney. Adapted from Ros Leibensperger, Cornell Theory
 *   Center. Converted to MPI: George L. Gusciora, MHPCC (1/95) 
 * LAST REVISED: 06/13/13 Blaise Barney
**********************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

void srandom (unsigned seed);
double dboard (int darts);
#define DARTS 50000     /* number of throws at dartboard */
#define ROUNDS 100      /* number of times "darts" is iterated */
#define MASTER 0        /* task ID of master task */

int main (int argc, char *argv[])
{
double	homepi,         /* value of pi calculated by current task */
	pisum,	        /* sum of tasks' pi values */
	pi,	        /* average of pi after "darts" is thrown */
	avepi;	        /* average pi value for all iterations */
int	taskid,	        /* task ID - also used as seed number */
	numtasks,       /* number of tasks */
	rc,             /* return code */
	i;
MPI_Status status;
double start, end;

/* Obtain number of tasks and task ID */
MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

if(taskid == MASTER)
   start = MPI_Wtime();

printf ("MPI task %d has started...\n", taskid);

/* Set seed for random number generator equal to task ID */
srandom (taskid);

avepi = 0;
for (i = 0; i < ROUNDS; i++) {
   /* All tasks calculate pi using dartboard algorithm */
   homepi = dboard(DARTS);

   /* Use MPI_Reduce to sum values of homepi across all tasks 
    * Master will store the accumulated value in pisum 
    * - homepi is the send buffer
    * - pisum is the receive buffer (used by the receiving task only)
    * - the size of the message is sizeof(double)
    * - MASTER is the task that will receive the result of the reduction
    *   operation
    * - MPI_SUM is a pre-defined reduction function (double-precision
    *   floating-point vector addition).  Must be declared extern.
    * - MPI_COMM_WORLD is the group of tasks that will participate.
    */

   rc = MPI_Reduce(&homepi, &pisum, 1, MPI_DOUBLE, MPI_SUM,
                   MASTER, MPI_COMM_WORLD);

   /* Master computes average for this iteration and all iterations */
   if (taskid == MASTER) {
      pi = pisum/numtasks;
      avepi = ((avepi * i) + pi)/(i + 1); 
      printf("   After %8d throws, average value of pi = %10.8f\n",
              (DARTS * (i + 1)),avepi);
   }    
} 
if (taskid == MASTER) {
   printf ("\nReal value of PI: 3.1415926535897 \n");
}
   
MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
end = MPI_Wtime();

if (taskid == MASTER) { /* use time on master node */
    printf("Runtime = %f\n", end-start);
}

MPI_Finalize();
return 0;
}



/**************************************************************************
* subroutine dboard
* DESCRIPTION:
*   Used in pi calculation example codes. 
*   See mpi_pi_send.c and mpi_pi_reduce.c  
*   Throw darts at board.  Done by generating random numbers 
*   between 0 and 1 and converting them to values for x and y 
*   coordinates and then testing to see if they "land" in 
*   the circle."  If so, score is incremented.  After throwing the 
*   specified number of darts, pi is calculated.  The computed value 
*   of pi is returned as the value of this function, dboard. 
*
*   Explanation of constants and variables used in this function:
*   darts       = number of throws at dartboard
*   score       = number of darts that hit circle
*   n           = index variable
*   r           = random number scaled between 0 and 1
*   x_coord     = x coordinate, between -1 and 1
*   x_sqr       = square of x coordinate
*   y_coord     = y coordinate, between -1 and 1
*   y_sqr       = square of y coordinate
*   pi          = computed value of pi
****************************************************************************/

double dboard(int darts)
{
#define sqr(x)	((x)*(x))
long random(void);
double x_coord, y_coord, pi, r; 
int score, n;
unsigned int cconst;  /* must be 4-bytes in size */
/*************************************************************************
 * The cconst variable must be 4 bytes. We check this and bail if it is
 * not the right size
 ************************************************************************/
if (sizeof(cconst) != 4) {
   printf("Wrong data size for cconst variable in dboard routine!\n");
   printf("See comments in source file. Quitting.\n");
   exit(1);
   }
   /* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
   cconst = 2 << (31 - 1);
   score = 0;

   /* "throw darts at board" */
   for (n = 1; n <= darts; n++)  {
      /* generate random numbers for x and y coordinates */
      r = (double)random()/cconst;
      x_coord = (2.0 * r) - 1.0;
      r = (double)random()/cconst;
      y_coord = (2.0 * r) - 1.0;

      /* if dart lands in circle, increment score */
      if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
           score++;
      }

/* calculate pi */
pi = 4.0 * (double)score/(double)darts;
return(pi);
} 
```
**Sample output:**
```bash
MPI task 0 has started...
   After    50000 throws, average value of pi = 3.14344000
   After   100000 throws, average value of pi = 3.14376000
   After   150000 throws, average value of pi = 3.14330667
   After   200000 throws, average value of pi = 3.14384000
   After   250000 throws, average value of pi = 3.14427200
   After   300000 throws, average value of pi = 3.14362667
   After   350000 throws, average value of pi = 3.14387429
   After   400000 throws, average value of pi = 3.14231000
   After   450000 throws, average value of pi = 3.14185778
   After   500000 throws, average value of pi = 3.14134400
   After   550000 throws, average value of pi = 3.14254545
   After   600000 throws, average value of pi = 3.14369333
   After   650000 throws, average value of pi = 3.14371077
   After   700000 throws, average value of pi = 3.14372000
   After   750000 throws, average value of pi = 3.14355733
   After   800000 throws, average value of pi = 3.14323000
   After   850000 throws, average value of pi = 3.14400941
   After   900000 throws, average value of pi = 3.14388889
   After   950000 throws, average value of pi = 3.14332211
   After  1000000 throws, average value of pi = 3.14287200
   After  1050000 throws, average value of pi = 3.14297905
   After  1100000 throws, average value of pi = 3.14294545
   After  1150000 throws, average value of pi = 3.14271304
   After  1200000 throws, average value of pi = 3.14224333
   After  1250000 throws, average value of pi = 3.14160960
   After  1300000 throws, average value of pi = 3.14196923
   After  1350000 throws, average value of pi = 3.14232889
   After  1400000 throws, average value of pi = 3.14248000
   After  1450000 throws, average value of pi = 3.14225655
   After  1500000 throws, average value of pi = 3.14239733
   After  1550000 throws, average value of pi = 3.14208774
   After  1600000 throws, average value of pi = 3.14218250
   After  1650000 throws, average value of pi = 3.14214545
   After  1700000 throws, average value of pi = 3.14215529
   After  1750000 throws, average value of pi = 3.14229486
   After  1800000 throws, average value of pi = 3.14201556
   After  1850000 throws, average value of pi = 3.14215135
   After  1900000 throws, average value of pi = 3.14208211
   After  1950000 throws, average value of pi = 3.14229333
   After  2000000 throws, average value of pi = 3.14237200
   After  2050000 throws, average value of pi = 3.14233171
   After  2100000 throws, average value of pi = 3.14223238
   After  2150000 throws, average value of pi = 3.14191070
   After  2200000 throws, average value of pi = 3.14196182
   After  2250000 throws, average value of pi = 3.14194667
   After  2300000 throws, average value of pi = 3.14200000
   After  2350000 throws, average value of pi = 3.14179915
   After  2400000 throws, average value of pi = 3.14189000
   After  2450000 throws, average value of pi = 3.14181878
   After  2500000 throws, average value of pi = 3.14189760
   After  2550000 throws, average value of pi = 3.14207529
   After  2600000 throws, average value of pi = 3.14241846
   After  2650000 throws, average value of pi = 3.14243472
   After  2700000 throws, average value of pi = 3.14254222
   After  2750000 throws, average value of pi = 3.14253236
   After  2800000 throws, average value of pi = 3.14259571
   After  2850000 throws, average value of pi = 3.14255719
   After  2900000 throws, average value of pi = 3.14272690
   After  2950000 throws, average value of pi = 3.14281085
   After  3000000 throws, average value of pi = 3.14282267
   After  3050000 throws, average value of pi = 3.14288262
   After  3100000 throws, average value of pi = 3.14268774
   After  3150000 throws, average value of pi = 3.14255873
   After  3200000 throws, average value of pi = 3.14266875
   After  3250000 throws, average value of pi = 3.14261908
   After  3300000 throws, average value of pi = 3.14257697
   After  3350000 throws, average value of pi = 3.14269851
   After  3400000 throws, average value of pi = 3.14252118
   After  3450000 throws, average value of pi = 3.14263188
   After  3500000 throws, average value of pi = 3.14269600
   After  3550000 throws, average value of pi = 3.14251155
   After  3600000 throws, average value of pi = 3.14257333
   After  3650000 throws, average value of pi = 3.14256438
   After  3700000 throws, average value of pi = 3.14240108
   After  3750000 throws, average value of pi = 3.14219520
   After  3800000 throws, average value of pi = 3.14213368
   After  3850000 throws, average value of pi = 3.14194078
   After  3900000 throws, average value of pi = 3.14200000
   After  3950000 throws, average value of pi = 3.14202734
   After  4000000 throws, average value of pi = 3.14210600
   After  4050000 throws, average value of pi = 3.14216395
   After  4100000 throws, average value of pi = 3.14211024
   After  4150000 throws, average value of pi = 3.14211759
   After  4200000 throws, average value of pi = 3.14216000
   After  4250000 throws, average value of pi = 3.14224188
   After  4300000 throws, average value of pi = 3.14217302
   After  4350000 throws, average value of pi = 3.14215080
   After  4400000 throws, average value of pi = 3.14211727
   After  4450000 throws, average value of pi = 3.14221124
   After  4500000 throws, average value of pi = 3.14228889
   After  4550000 throws, average value of pi = 3.14230242
   After  4600000 throws, average value of pi = 3.14219913
   After  4650000 throws, average value of pi = 3.14231828
   After  4700000 throws, average value of pi = 3.14236255
   After  4750000 throws, average value of pi = 3.14231663
   After  4800000 throws, average value of pi = 3.14229500
   After  4850000 throws, average value of pi = 3.14230515
   After  4900000 throws, average value of pi = 3.14243918
   After  4950000 throws, average value of pi = 3.14246384
   After  5000000 throws, average value of pi = 3.14243120

Real value of PI: 3.1415926535897 
Runtime = 0.246764
```
### mpi-parent.c
```c
/**********************************************************************
 * FILE: mpi_pi_reduce.c
 * OTHER FILES: dboard.c
 * DESCRIPTION:
 *   MPI pi Calculation Example - C Version
 *   Collective Communication example:
 *   This program calculates pi using a "dartboard" algorithm.  See
 *   Fox et al.(1988) Solving Problems on Concurrent Processors, vol.1
 *   page 207.  All processes contribute to the calculation, with the
 *   master averaging the values for pi. This version uses mpc_reduce to
 *   collect results
 * AUTHOR: Blaise Barney. Adapted from Ros Leibensperger, Cornell Theory
 *   Center. Converted to MPI: George L. Gusciora, MHPCC (1/95)
 * LAST REVISED: 06/13/13 Blaise Barney
**********************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

void srandom (unsigned seed);
double dboard (int darts);
#define DARTS 50000     /* number of throws at dartboard */
#define ROUNDS 100      /* number of times "darts" is iterated */
#define MASTER 0        /* task ID of master task */

int main (int argc, char *argv[])
{
double  homepi,         /* value of pi calculated by current task */
        pisum,          /* sum of tasks' pi values */
        pi,             /* average of pi after "darts" is thrown */
        avepi;          /* average pi value for all iterations */
int     taskid,         /* task ID - also used as seed number */
        numtasks,       /* number of tasks */
        rc,             /* return code */
        i;
MPI_Status status;

if(argc != 2)
{
   printf("usage: mpi-parent.run n; where n is an int\n");
   error("bad input\n");
}

numtasks = 10;
double start, end;
MPI_Comm intercomm;

/* Obtain number of tasks and task ID */
MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

if(taskid == MASTER)
   start = MPI_Wtime();

printf ("MPI task %d has started...\n", taskid);
printf("MASTER NOW STARTING %d CHILDREN...\n", numtasks);

MPI_Comm_spawn("mpi-child.run", MPI_ARGV_NULL, 1000, MPI_INFO_NULL, MASTER, MPI$


/* Set seed for random number generator equal to task ID */
srandom (taskid);

avepi = 0;
for (i = 0; i < ROUNDS; i++) {

   rc = MPI_Reduce(&homepi, &pisum, 1, MPI_DOUBLE, MPI_SUM,
                   MPI_ROOT, intercomm);

   /* Master computes average for this iteration and all iterations */
   if (taskid == MASTER) {
      pi = pisum/1000;
      avepi = ((avepi * i) + pi)/(i + 1);
      printf("   After %8d throws, average value of pi = %10.8f\n",
              (DARTS * (i + 1)),avepi);
   }
}
if (taskid == MASTER) {
   printf ("\nReal value of PI: 3.1415926535897 \n");
}

MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
end = MPI_Wtime();

if (taskid == MASTER) { /* use time on master node */
    printf("Runtime = %f\n", end-start);
}

MPI_Finalize();
return 0;
}
```
### mpi-child.c
```c
/**********************************************************************
 * FILE: mpi_pi_reduce.c
 * OTHER FILES: dboard.c
 * DESCRIPTION:  
 *   MPI pi Calculation Example - C Version 
 *   Collective Communication example:  
 *   This program calculates pi using a "dartboard" algorithm.  See
 *   Fox et al.(1988) Solving Problems on Concurrent Processors, vol.1
 *   page 207.  All processes contribute to the calculation, with the
 *   master averaging the values for pi. This version uses mpc_reduce to 
 *   collect results
 * AUTHOR: Blaise Barney. Adapted from Ros Leibensperger, Cornell Theory
 *   Center. Converted to MPI: George L. Gusciora, MHPCC (1/95) 
 * LAST REVISED: 06/13/13 Blaise Barney
**********************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

void srandom (unsigned seed);
double dboard (int darts);
#define DARTS 50000     /* number of throws at dartboard */
#define ROUNDS 100      /* number of times "darts" is iterated */
#define MASTER 0        /* task ID of master task */

int main (int argc, char *argv[])
{
double	homepi,         /* value of pi calculated by current task */
	pisum,	        /* sum of tasks' pi values */
	pi,	        /* average of pi after "darts" is thrown */
	avepi;	        /* average pi value for all iterations */
int	taskid,	        /* task ID - also used as seed number */
	numtasks,       /* number of tasks */
	rc,             /* return code */
	i;
MPI_Status status;

int size;
double start, end;
MPI_Comm parentcomm;

/* Obtain number of tasks and task ID */
MPI_Init(&argc,&argv);
MPI_Comm_get_parent(&parentcomm);
if(parentcomm == MPI_COMM_NULL)
{
   error("MPI child task %d has no parent!?", taskid);
}

MPI_Comm_remote_size(parentcomm, &size); 
if (size != 1)
{
   error("Something's wrong with the parent");
}

MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
printf ("MPI child task %d has started...\n", taskid);

/* Set seed for random number generator equal to task ID */
srandom (taskid);

avepi = 0;
for (i = 0; i < ROUNDS; i++) {
   /* All tasks calculate pi using dartboard algorithm */
   homepi = dboard(DARTS);

   /* Use MPI_Reduce to sum values of homepi across all tasks 
    * Master will store the accumulated value in pisum 
    * - homepi is the send buffer
    * - pisum is the receive buffer (used by the receiving task only)
    * - the size of the message is sizeof(double)
    * - MASTER is the task that will receive the result of the reduction
    *   operation
    * - MPI_SUM is a pre-defined reduction function (double-precision
    *   floating-point vector addition).  Must be declared extern.
    * - MPI_COMM_WORLD is the group of tasks that will participate.
    */

   rc = MPI_Reduce(&homepi, &pisum, 1, MPI_DOUBLE, MPI_SUM,
                   MASTER, parentcomm);  
}

MPI_Finalize();
return 0;
}



/**************************************************************************
* subroutine dboard
* DESCRIPTION:
*   Used in pi calculation example codes. 
*   See mpi_pi_send.c and mpi_pi_reduce.c  
*   Throw darts at board.  Done by generating random numbers 
*   between 0 and 1 and converting them to values for x and y 
*   coordinates and then testing to see if they "land" in 
*   the circle."  If so, score is incremented.  After throwing the 
*   specified number of darts, pi is calculated.  The computed value 
*   of pi is returned as the value of this function, dboard. 
*
*   Explanation of constants and variables used in this function:
*   darts       = number of throws at dartboard
*   score       = number of darts that hit circle
*   n           = index variable
*   r           = random number scaled between 0 and 1
*   x_coord     = x coordinate, between -1 and 1
*   x_sqr       = square of x coordinate
*   y_coord     = y coordinate, between -1 and 1
*   y_sqr       = square of y coordinate
*   pi          = computed value of pi
****************************************************************************/

double dboard(int darts)
{
#define sqr(x)	((x)*(x))
long random(void);
double x_coord, y_coord, pi, r; 
int score, n;
unsigned int cconst;  /* must be 4-bytes in size */
/*************************************************************************
 * The cconst variable must be 4 bytes. We check this and bail if it is
 * not the right size
 ************************************************************************/
if (sizeof(cconst) != 4) {
   printf("Wrong data size for cconst variable in dboard routine!\n");
   printf("See comments in source file. Quitting.\n");
   exit(1);
   }
   /* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
   cconst = 2 << (31 - 1);
   score = 0;

   /* "throw darts at board" */
   for (n = 1; n <= darts; n++)  {
      /* generate random numbers for x and y coordinates */
      r = (double)random()/cconst;
      x_coord = (2.0 * r) - 1.0;
      r = (double)random()/cconst;
      y_coord = (2.0 * r) - 1.0;

      /* if dart lands in circle, increment score */
      if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
           score++;
      }

/* calculate pi */
pi = 4.0 * (double)score/(double)darts;
return(pi);
}
```

**Sample output:**
```bash
[apini] [/home/c/c_mcart/Documents] > mpirun -n 1 mpi-parent.run 2
MPI task 0 has started...
MASTER NOW STARTING 1 CHILDREN...
MPI child task 0 has started...
MPI child task 1 has started...
   After    50000 throws, average value of pi = 3.14344000
   After   100000 throws, average value of pi = 3.14376000
   After   150000 throws, average value of pi = 3.14330667
   After   200000 throws, average value of pi = 3.14384000
   After   250000 throws, average value of pi = 3.14427200
   After   300000 throws, average value of pi = 3.14362667
   After   350000 throws, average value of pi = 3.14387429
   After   400000 throws, average value of pi = 3.14231000
   After   450000 throws, average value of pi = 3.14185778
   After   500000 throws, average value of pi = 3.14134400
   After   550000 throws, average value of pi = 3.14254545
   After   600000 throws, average value of pi = 3.14369333
   After   650000 throws, average value of pi = 3.14371077
   After   700000 throws, average value of pi = 3.14372000
   After   750000 throws, average value of pi = 3.14355733
   After   800000 throws, average value of pi = 3.14323000
   After   850000 throws, average value of pi = 3.14400941
   After   900000 throws, average value of pi = 3.14388889
   After   950000 throws, average value of pi = 3.14332211
   After  1000000 throws, average value of pi = 3.14287200
   After  1050000 throws, average value of pi = 3.14297905
   After  1100000 throws, average value of pi = 3.14294545
   After  1150000 throws, average value of pi = 3.14271304
   After  1200000 throws, average value of pi = 3.14224333
   After  1250000 throws, average value of pi = 3.14160960
   After  1300000 throws, average value of pi = 3.14196923
   After  1350000 throws, average value of pi = 3.14232889
   After  1400000 throws, average value of pi = 3.14248000
   After  1450000 throws, average value of pi = 3.14225655
   After  1500000 throws, average value of pi = 3.14239733
   After  1550000 throws, average value of pi = 3.14208774
   After  1600000 throws, average value of pi = 3.14218250
   After  1650000 throws, average value of pi = 3.14214545
   After  1700000 throws, average value of pi = 3.14215529
   After  1750000 throws, average value of pi = 3.14229486
   After  1800000 throws, average value of pi = 3.14201556
   After  1850000 throws, average value of pi = 3.14215135
   After  1900000 throws, average value of pi = 3.14208211
   After  1950000 throws, average value of pi = 3.14229333
   After  2000000 throws, average value of pi = 3.14237200
   After  2050000 throws, average value of pi = 3.14233171
   After  2100000 throws, average value of pi = 3.14223238
   After  2150000 throws, average value of pi = 3.14191070
   After  2200000 throws, average value of pi = 3.14196182
   After  2250000 throws, average value of pi = 3.14194667
   After  2300000 throws, average value of pi = 3.14200000
   After  2350000 throws, average value of pi = 3.14179915
   After  2400000 throws, average value of pi = 3.14189000
   After  2450000 throws, average value of pi = 3.14181878
   After  2500000 throws, average value of pi = 3.14189760
   After  2550000 throws, average value of pi = 3.14207529
   After  2600000 throws, average value of pi = 3.14241846
   After  2650000 throws, average value of pi = 3.14243472
   After  2700000 throws, average value of pi = 3.14254222
   After  2750000 throws, average value of pi = 3.14253236
   After  2800000 throws, average value of pi = 3.14259571
   After  2850000 throws, average value of pi = 3.14255719
   After  2900000 throws, average value of pi = 3.14272690
   After  2950000 throws, average value of pi = 3.14281085
   After  3000000 throws, average value of pi = 3.14282267
   After  3050000 throws, average value of pi = 3.14288262
   After  3100000 throws, average value of pi = 3.14268774
   After  3150000 throws, average value of pi = 3.14255873
   After  3200000 throws, average value of pi = 3.14266875
   After  3250000 throws, average value of pi = 3.14261908
   After  3300000 throws, average value of pi = 3.14257697
   After  3350000 throws, average value of pi = 3.14269851
   After  3400000 throws, average value of pi = 3.14252118
   After  3450000 throws, average value of pi = 3.14263188
   After  3500000 throws, average value of pi = 3.14269600
   After  3550000 throws, average value of pi = 3.14251155
   After  3600000 throws, average value of pi = 3.14257333
   After  3650000 throws, average value of pi = 3.14256438
   After  3700000 throws, average value of pi = 3.14240108
   After  3750000 throws, average value of pi = 3.14219520
   After  3800000 throws, average value of pi = 3.14213368
   After  3850000 throws, average value of pi = 3.14194078
   After  3900000 throws, average value of pi = 3.14200000
   After  3950000 throws, average value of pi = 3.14202734
   After  4000000 throws, average value of pi = 3.14210600
   After  4050000 throws, average value of pi = 3.14216395
   After  4100000 throws, average value of pi = 3.14211024
   After  4150000 throws, average value of pi = 3.14211759
   After  4200000 throws, average value of pi = 3.14216000
   After  4250000 throws, average value of pi = 3.14224188
   After  4300000 throws, average value of pi = 3.14217302
   After  4350000 throws, average value of pi = 3.14215080
   After  4400000 throws, average value of pi = 3.14211727
   After  4450000 throws, average value of pi = 3.14221124
   After  4500000 throws, average value of pi = 3.14228889
   After  4550000 throws, average value of pi = 3.14230242
   After  4600000 throws, average value of pi = 3.14219913
   After  4650000 throws, average value of pi = 3.14231828
   After  4700000 throws, average value of pi = 3.14236255
   After  4750000 throws, average value of pi = 3.14231663
   After  4800000 throws, average value of pi = 3.14229500
   After  4850000 throws, average value of pi = 3.14230515
   After  4900000 throws, average value of pi = 3.14243918
   After  4950000 throws, average value of pi = 3.14246384
   After  5000000 throws, average value of pi = 3.14243120

Real value of PI: 3.1415926535897 
Runtime = 1.304495
```

## Attributions
* Q1-A: (https://stackoverflow.com/a/5301414)
* Q1-D: (http://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-2.0/node98.htm)
