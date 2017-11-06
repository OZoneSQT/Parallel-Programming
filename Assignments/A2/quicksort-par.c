#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>      // this is so memcopy doesnt produce undeclaired warnings =?

#define MASTER 0        /* task ID of master task */
MPI_Comm comm = MPI_COMM_WORLD;

void swap(int* a, int* b);
int partition(int arr[], int low, int high);
void quickSort(int arr[], int low, int high);
void printArray(int arr[], int size);

int log2(int x);
int randomized_select(int *A, int p, int r, int i);
int par_partition(int* Arr, int ptr_offset, int ptr_limit, int active_dimension);
void hypercube_quicksort(int *A, int n);

int main(int argc, char *argv[])
{
   int taskid;                                  // task ID - also used as seed number
   int numtasks;                                // number of tasks

   int random_array[] = { 33, 324, 212, 358, 185, 398, 175, 418, 64, 20, 271, 310, 406, 19, 10, 413, 430, 143, 461, 493, 388, 315, 157, 335, 391, 116, 112, 129, 149, 110, 179, 187, 459, 434, 479, 225, 359, 201, 2, 159, 177, 236, 83, 238, 153, 231, 68, 31, 124, 474, 245, 447, 118, 319, 300, 377, 148, 407, 200, 273, 437, 215, 376, 360, 381, 384, 297, 1, 107, 145, 184, 89, 119, 443, 209, 383, 291, 232, 211, 125, 229, 432, 340, 390, 279, 429, 24, 76, 353, 414, 425, 343, 37, 139, 67, 294, 370, 400, 369, 9, 221, 223, 349, 280, 455, 50, 299, 144, 77, 4, 346, 120, 492, 150, 91, 275, 444, 167, 309, 100, 241, 32, 500, 378, 259, 366, 3, 287, 289, 302, 318, 475, 87, 62, 423, 26, 488, 466, 278, 204, 472, 189, 426, 354, 181, 332, 163, 202, 94, 441 };
   int arr_size = sizeof(random_array) / sizeof(random_array[0]);

   MPI_Status status;
   double start, end;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

   printf("MPI task %d has started...\n", taskid);

   MPI_Barrier(MPI_COMM_WORLD);                 /* IMPORTANT */
   if (taskid == MASTER)
   {
      start = MPI_Wtime();

      printf("Unorted array: ");
      printArray(random_array, arr_size);
   }

   // Inspiration: https://github.com/examachine/par-quicksort/blob/ab1dce68e23657902879bb3641009cffe5ea043b/quicksort.c#L184
   hypercube_quicksort(random_array, arr_size - 1);

   MPI_Barrier(MPI_COMM_WORLD);                 /* IMPORTANT */
   if (taskid == MASTER)
   {
      end = MPI_Wtime();

      printf("Sorted array: ");
      printArray(random_array, arr_size);

      printf("Runtime = %f\n", end - start);
   }

   MPI_Finalize();
   return 0;
}

/*
    THE FOLLOWING FUNCTIONS ARE _HEAVILY_ MODIFIED FROM
    examachine/par-quicksort REPOSITORY.
*/

void hypercube_quicksort(int *Arr, int size)
{
   int taskid;
   int numprocs;
   int dimension;

   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

   // find number of dimensions
   dimension = log2(numprocs);
   if (taskid == MASTER) printf("%d processors - hypercube has %d dimensions\n", numprocs, dimension);

   for (int active_dimension = dimension - 1; active_dimension >= 0; active_dimension--)
   {
      int part_at_index = par_partition(Arr, 0, size - 1, active_dimension);
      int a1size = part_at_index + 1;
      int a2size = size - (part_at_index + 1);
      int newsize = 0;

      printf("%d partition done, step %d\n", taskid, active_dimension);

      int link = 1 << active_dimension;
      if (taskid & link)                      // ith bit == 1
      {
         // this subcube has the large numbers --> send A1 (smaller)
         int b2size;

         /* exchange sizes */
         printf("ith bit 1: sending from %d to %d\n", taskid, taskid^link);
         MPI_Recv(&b2size, 1, MPI_INT, taskid^link, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         MPI_Send(&a1size, 1, MPI_INT, taskid^link, 0, MPI_COMM_WORLD);
         printf("%d: am high: size=%d || partner %d: size=%d\n", taskid, a1size, taskid^link, b2size);

         /* receive B2 */
         newsize = a2size + b2size;
         int* B = (int*)malloc(newsize * sizeof(int));
         if (B == NULL)
         {
            printf("alloc failed!!!!\n");
            exit(-1);
         }
         memcpy(B, Arr + a1size, a2size * sizeof(int));
         MPI_Recv(B + a2size, b2size, MPI_INT, taskid^link, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

         /* send */
         MPI_Send(Arr, a1size, MPI_INT, taskid^link, 0, MPI_COMM_WORLD);

         free(Arr);
         Arr = B;
         size = newsize;
      }
      else
      {
         // this sub-cube has the small numbers --> send A2 (larger)
         int b1size;

         /* exchange sizes */
         printf("ith bit 0: sending from %d to %d\n", taskid, taskid^link);
         MPI_Send(&a2size, 1, MPI_INT, taskid^link, 0, MPI_COMM_WORLD);
         MPI_Recv(&b1size, 1, MPI_INT, taskid^link, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         printf("%d: am low: size=%d || partner %d: size=%d\n", taskid, a2size, taskid^link, b1size);

         /* send A2 */
         MPI_Send(Arr + a1size, a2size, MPI_INT, taskid^link, 0, MPI_COMM_WORLD);

         /* receive B1 */
         newsize = a1size + b1size;
         int* B = (int*)malloc(newsize * sizeof(int));
         if (B == NULL)
         {
            printf("alloc failed!!!!\n");
            exit(-1);
         }
         memcpy(B, Arr, part_at_index * sizeof(int));
         MPI_Recv(B + part_at_index, b1size, MPI_INT, taskid^link, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

         free(Arr);
         Arr = B;
         size = newsize;
      }
   }

   quickSort(Arr, 0, size - 1);
}


int par_partition(int* Arr, int ptr_offset, int ptr_limit, int active_dimension)
{
   const int COLOR_HIGH = 1;
   const int COLOR_LOW = 0;

   int local_pivot, global_pivot, nprocs, taskid;
   int color;
   int size = ptr_limit - ptr_offset;

   MPI_Comm_size(comm, &nprocs);
   MPI_Comm_rank(comm, &taskid);

   // select pivot - median
   {
      printf("%d: selecting local pivot\n", taskid);
      local_pivot = rand()%475;
      printf("%d: local pivot is %d\n", taskid, local_pivot);
   }

   // take average of pivots
   {
      int SUM = 0;
      MPI_Allreduce(&local_pivot, &SUM, 1, MPI_INT, MPI_SUM, comm);
      global_pivot = SUM / nprocs;
      if (taskid == MASTER) printf("%d: pivot is %d\n", taskid, global_pivot);

      if (taskid & (1 << active_dimension)) // determine high vs low group
         color = COLOR_HIGH;
      else
         color = COLOR_LOW;

      MPI_Comm newcomm;
      if (MPI_Comm_split(comm, color, taskid, &newcomm) != MPI_SUCCESS) {
         printf("\n\n ---- ERROR SPLITTING COMM ----\n\n");
         exit(-1);
      }

      comm = newcomm;
   }

   int i = ptr_offset - 1;
   int j = ptr_limit + 1;

   while (1) {
      //find range limits
      do {
         j = j - 1;                         // find lower above pivot on upper
      } while (Arr[j] > global_pivot);
      do {
         i = i + 1;
      } while (Arr[i] < global_pivot);      // fine higher below pivot on lower

      if (i < j)                            // if on each half
         swap(&Arr[i], &Arr[j]);            // than swap
      else
         return j;                          // done!
   }
}

int randomized_select(int *A, int ptr_offset, int ptr_limit, int active_dimension)
{
   if (ptr_offset == ptr_limit)
      return A[ptr_offset];

   int part_at_index = partition(A, ptr_offset, ptr_limit);
   int new_size = part_at_index - ptr_offset + 1;

   if (active_dimension <= new_size)
      return randomized_select(A, ptr_offset, part_at_index, active_dimension);
   else
      return randomized_select(A, part_at_index + 1, ptr_limit, active_dimension - new_size);
}

int log2(int x)
{
   int result, mask;

   if (x <= 0)
      return 0;
   result = 31;

   for (mask = (1L << 31); result >= 0; result--, (mask >>= 1))
   {
      if (mask&x)
         return result;
   }

   return 0;
}

/*
    THE CODE BELOW IS TAKEN FROM http://www.geeksforgeeks.org/quick-sort/
    ALL CREDIT GOES TO THEM FOR THE CODE
*/

// A utility function to swap two elements
void swap(int* a, int* b)
{
   int t = *a;
   *a = *b;
   *b = t;
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
   array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition(int arr[], int low, int high)
{
   int pivot = arr[high];    // pivot
   int i = (low - 1);        // Index of smaller element

   for (int j = low; j <= high - 1; j++)
   {
      // If current element is smaller than or equal to pivot
      if (arr[j] <= pivot)
      {
         i++;              // increment index of smaller element
         swap(&arr[i], &arr[j]);
      }
   }

   swap(&arr[i + 1], &arr[high]);
   return (i + 1);
}

/* The main function that implements QuickSort
   arr[] --> Array to be sorted,
   low  --> Starting index,
   high  --> Ending index */
void quickSort(int arr[], int low, int high)
{
   if (low < high)
   {
      /* pi is partitioning index, arr[p] is now
         at right place */
      int pi = partition(arr, low, high);

      // Separately sort elements before
      // partition and after partition
      quickSort(arr, low, pi - 1);
      quickSort(arr, pi + 1, high);
   }
}

/* Function to print an array */
void printArray(int arr[], int size)
{
   int i;
   for (i = 0; i < size; i++)
      printf("%d ", arr[i]);
   printf("\n");
}
