#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0        /* task ID of master task */

void swap(int* a, int* b);
int partition (int arr[], int low, int high);
void quickSort(int arr[], int low, int high);
void printArray(int arr[], int size);

int main (int argc, char *argv[])
{
int	  taskid;	        /* task ID - also used as seed number */
int  	numtasks;       /* number of tasks */
int  	rc,             /* return code from MPI_REDUCE */

int random_array_seq[] = { 33, 324, 212, 358, 185, 398, 175, 418, 64, 20, 271, 310, 406, 19, 10, 413, 430, 143, 461, 493, 388, 315, 157, 335, 391, 116, 112, 129, 149, 110, 179, 187, 459, 434, 479, 225, 359, 201, 2, 159, 177, 236, 83, 238, 153, 231, 68, 31, 124, 474, 245, 447, 118, 319, 300, 377, 148, 407, 200, 273, 437, 215, 376, 360, 381, 384, 297, 1, 107, 145, 184, 89, 119, 443, 209, 383, 291, 232, 211, 125, 229, 432, 340, 390, 279, 429, 24, 76, 353, 414, 425, 343, 37, 139, 67, 294, 370, 400, 369, 9, 221, 223, 349, 280, 455, 50, 299, 144, 77, 4, 346, 120, 492, 150, 91, 275, 444, 167, 309, 100, 241, 32, 500, 378, 259, 366, 3, 287, 289, 302, 318, 475, 87, 62, 423, 26, 488, 466, 278, 204, 472, 189, 426, 354, 181, 332, 163, 202, 94, 441 };
int random_array1[] = { 424, 453, 66, 438, 416, 22, 170, 86, 312, 195, 309, 333, 420, 47, 212, 53, 460, 9, 273, 130, 150, 458, 63, 214, 95, 144, 254, 68, 481, 252, 127, 80, 19, 140, 38, 441, 226, 256, 193, 119, 168, 169, 79, 343, 446, 386, 42, 155, 96, 37, 78, 495, 227, 194, 106, 89, 457, 496, 351, 228, 18, 336, 334, 431, 277, 57, 149, 184, 43, 61, 72, 87, 320, 387, 403, 406, 109, 245, 439, 115, 116, 482, 364, 435, 308, 103, 347, 222, 342, 265, 382, 306, 55, 314, 35, 163, 51, 331, 345, 208, 282, 20, 126, 48, 54, 338, 238, 461, 428, 445, 395, 1, 292, 117, 118, 269, 423, 154, 381, 385, 348, 278, 409, 249, 197, 448, 307, 356, 139, 202, 178, 177, 321, 142, 366, 161, 337, 410, 489, 426, 305, 207, 464, 362, 182, 21, 373, 248, 369, 459 };
int random_array2[] = { 24, 364, 432, 215, 411, 210, 371, 94, 80, 202, 23, 308, 262, 250, 291, 469, 260, 29, 310, 205, 201, 9, 384, 292, 244, 463, 295, 496, 122, 182, 245, 154, 62, 77, 231, 98, 134, 59, 350, 187, 249, 267, 118, 414, 341, 206, 446, 148, 139, 138, 407, 306, 388, 177, 307, 6, 330, 167, 273, 125, 436, 79, 102, 22, 376, 203, 13, 181, 325, 55, 299, 149, 136, 460, 36, 470, 274, 465, 422, 481, 127, 323, 352, 329, 101, 96, 450, 442, 70, 74, 455, 178, 83, 179, 34, 17, 328, 309, 486, 419, 349, 192, 246, 404, 46, 184, 121, 453, 99, 443, 286, 69, 480, 170, 491, 40, 14, 230, 112, 327, 293, 483, 120, 471, 447, 454, 196, 76, 421, 395, 426, 375, 159, 271, 21, 160, 285, 237, 381, 413, 251, 229, 241, 297, 338, 319, 186, 150, 492, 401 };

MPI_Status status;
double start, end;

/* Obtain number of tasks and task ID */
MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

printf ("MPI task %d has started...\n", taskid);

MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
if(taskid == MASTER)
   start = MPI_Wtime();
{
   int n = sizeof(random_array_seq)/sizeof(random_array_seq[0]); // should be 150
   printf("Unorted array: n");
   printArray(random_array_seq, n);
   
   quickSort(random_array_seq, 0, n-1);
   
   printf("Sorted array: n");
   printArray(random_array_seq, n);
}
   
MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
if (taskid == MASTER) {
   end = MPI_Wtime();
   printf("Runtime = %f\n", end-start);
}

MPI_Finalize();
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
int partition (int arr[], int low, int high)
{
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            i++;    // increment index of smaller element
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
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("n");
}
