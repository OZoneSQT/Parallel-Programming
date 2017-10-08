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

