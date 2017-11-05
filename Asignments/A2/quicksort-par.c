/* parallel quicksort on hypercube */

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

MPI_Comm comm;
MPI_Comm newcomm;
FILE* debug;

#ifdef DEBUG
#define LOG(x...) fprintf(debug, ##x)
#else
#define LOG(x...) /* no comment */
#endif

void swap( int *x, int *y) {
  int tmp;

  tmp = *x;
  *x = *y;
  *y = tmp;
}

void permute_array(int *array, int size) {

  int i, aNumber;
  
  srand((unsigned)time(NULL));

  for (i=size-1; i>0; i--){
    aNumber = i*(float) random() / (float) 0x7fffffff;
    swap(&array[i], &array[aNumber]);
  }

}

void print_array(int* A, int n)
{
#ifdef DEBUG
  int i;
  for (i=0; i<n; i++) {
    fprintf(debug,"%d ", A[i]);
  }
  LOG("\n");
#endif
}

void quicksort(int *A, int p, int r)
{
  if (p < r) {
    int q = partition(A, p, r);
    quicksort(A, p, q);
    quicksort(A, q+1, r);
  }
}

int randomized_select(int *A, int p, int r, int i)
{
  int q;
  int k;
  if (p == r)
    return A[p];
  q = partition(A, p, r);
  k = q - p + 1;
  if (i<=k) 
    return randomized_select(A,p,q,i);
  else
    return randomized_select(A,q+1,r,i-k);
}

int partition(int *A, int p, int r)
{
  int x,i,j;
  /* select pivot randomly */
  x = A [ p + (int)( ((double)r-p)*rand()/(RAND_MAX + 1.0) ) ];

/*   int x = A[p]; */
  i = p - 1;
  j = r + 1;
  while (1) {
    do {
      j = j - 1;
    } while (A[j]>x);
    do {
      i = i + 1;
    } while (A[i]<x);
    if (i<j) {
      int tmp;
      tmp = A[i];
      A[i] = A[j];
      A[j] = tmp;
    }
    else
      return j;
  }
}

int par_partition(int *A, int p, int r, int cube)
{
  int x,i,j,nprocs,id;
  int color;
  int size = r-p;
  int *tmp;
  MPI_Comm_size(comm,&nprocs);
  MPI_Comm_rank(comm,&id);

  /* select pivot */

  /* average of first 3 */
/*   if (r-p>1) { */
/*     x = (A[p] + A[p+1] + A[p+2]) / 3; */
/*   } */
/*   else if (r-p>0) { */
/*     x = (A[p] + A[p+1]) / 2; */
/*   } */
/*   else */
/*     x = A[p]; */

  /* median */
  tmp = (int*)malloc(sizeof(int)*(size));
  memcpy(tmp, A+p, sizeof(int)*(size));
  x = randomized_select(tmp,0,size-1,size/2);

  /* take average of pivots */
  {
    int SUM = 0;
    MPI_Allreduce(&x, &SUM, 1, MPI_INT, MPI_SUM, comm);
    LOG("%d: local pivot is %d, sum is %d\n",id,x,SUM);

    x = SUM/nprocs;
    LOG("%d: pivot is %d\n",id,x);

    if (id & (1<<cube))
      color = 1;
    else
      color = 0;
    /* divide cube */
    LOG("split color %d, id %d, cube %d\n",color,id,cube);
/*     MPI_Barrier(comm); */
    if (MPI_Comm_split(comm, color, id,&newcomm)!=MPI_SUCCESS) {
      LOG("error, split\n");
      exit(-1);
    }
    comm = newcomm;
    MPI_Comm_size(comm,&nprocs);
    LOG("new size %d\n",nprocs);
  }
  /*int x = A[p];*/
  i = p - 1;
  j = r + 1;
  while (1) {
    do {
      j = j - 1;
    } while (A[j]>x);
    do {
      i = i + 1;
    } while (A[i]<x);
    if (i<j) {
      int tmp;
      tmp = A[i];
      A[i] = A[j];
      A[j] = tmp;
    }
    else
      return j;
  }
}

int log2(int x)
{
  int l, mask;
  if (x<=0)
    return 0;
  l=31;
  for (mask=(1L << 31); l>=0; l--, (mask>>=1)) {
    if (mask&x)
      return l;
  }
  return 0;
}


void hypercube_quicksort(int *A, int n)
{
  int id;
  int i;
  int numprocs;
  int d;
  int *B;			/* accumulation buffer */

  B = (int*)malloc(2*n*sizeof(int));

  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  // find number of dimensions
  d = log2(numprocs);
  printf ("%d processors\n",numprocs);
  printf ("hypercube has %d dimensions\n",d);
  for (i=d-1;i>=0;i--) {
    int q = par_partition(A,0,n-1,i);
    int link = 1<<i;
    int a1size = q+1;
    int a2size = n-(q+1);
    int b1size,b2size,newsize;
    printf("%d partition done, step %d\n",id,i); 
    LOG("%d: **** STEP %d partitioned array: ", id, i);
    print_array(A, n);
    /*printf ("link=%d\n",link);*/
    if (!(id & link)) {	/* ith bit 0 */
      /* this subcube has the small numbers */
      /* send A2 */

      /* exchange sizes */
      LOG("ith bit 0: sending from %d to %d\n", id, id^link);
      MPI_Send(&a2size, 1, MPI_INT, id^link, 0, MPI_COMM_WORLD);
      MPI_Recv(&b1size, 1, MPI_INT, id^link, 0, MPI_COMM_WORLD,
	       MPI_STATUS_IGNORE);

      /* send A2 */
      LOG("%d,low: a2size=%d, b1size=%d, partner %d\n",
	     id,a2size,b1size, id^link);
      MPI_Send(A+a1size, a2size, MPI_INT, id^link, 0, MPI_COMM_WORLD);
      LOG("%d: sent %d: ", id, id^link);
      print_array(A+q+1, a2size);

      /* receive B1 */
      newsize = a1size + b1size;
      LOG("A=");
      print_array(A, n);
      if (! (B = (int*)malloc(newsize*sizeof(int)))) {
	printf("alloc failed!!!!\n");
	exit(-1);
      }
      memcpy(B, A, (q+1)*sizeof(int));
      MPI_Recv(B+(q+1), b1size, MPI_INT, id^link, 0,
	       MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      free(A);
      A = B;
      n = newsize;
      LOG("%d: cumulative A1 %d, B1 %d: ", id, a1size, b1size);
      print_array(A, n);
    }
    else {
      /* this subcube has the large numbers */
      /* send A1 */

      /* exchange sizes */
      LOG("ith bit 1: sending from %d to %d\n",id, id^link);
      MPI_Recv(&b2size, 1, MPI_INT, id^link, 0, MPI_COMM_WORLD,
	       MPI_STATUS_IGNORE);
      MPI_Send(&a1size, 1, MPI_INT, id^link, 0, MPI_COMM_WORLD);
      LOG("%d,high: a1size=%d, b2size=%d, partner %d\n",
	     id,a1size,b2size, id^link);

      /* receive */
      newsize = a2size + b2size;
      if (! (B = (int*)malloc(newsize*sizeof(int)))) {
	printf("malloc failed!!!!\n");
	exit(-1);
      }
      LOG("A=");
      print_array(A, n);
      LOG("copying A2:");
      print_array(A+a1size, a2size);
      memcpy(B,A+a1size,a2size*sizeof(int));
      MPI_Recv(B+a2size, b2size, MPI_INT, id^link, 0, MPI_COMM_WORLD,
	       MPI_STATUS_IGNORE);

      /* send */
      MPI_Send(A, a1size, MPI_INT, id^link, 0, MPI_COMM_WORLD);
      LOG("%d: sent %d: ", id, id^link);
      print_array(A, a1size);

      free(A);
      A = B;
      n = newsize;
      printf("%d send/recv done, step %d\n",id,i); 
      LOG("%d: cumulative A2 %d, B2 %d: ", id, a2size, b2size);
      print_array(A, n);
    }
  }
  LOG("A=");
  print_array(A, n);
  quicksort(A,0,n-1); 
  LOG("%d: sorted array: ", id);
  print_array(A,n);
}


/* n = local size */
void cons_array_old(int *A, int id, int total_size, int n)
{
  int start, end;
  int i;
  start = n * id;
  end = start + n;
  LOG("n %d, start %d, end %d\n", n, start, end);
  for (i=0;i<total_size;i++) {
    rand();
    if ( (i>=start) && (i<end) ) {
      A[i-start] = (1000.0*rand()/(RAND_MAX+1.0));
    }
  }
}

/* n = local size */
void cons_array(int *A, int id, int total_size, int n)
{
  int start, end;
  int i;

  if (id==0) {
    for (i=0; i < total_size; i++)
      A[i] = i;
    permute_array(A, total_size);
  }

  MPI_Scatter(A, n, MPI_INT, A, n, MPI_INT, 0, MPI_COMM_WORLD);
}

void main(int argc, char **argv)
{
  int* A;
  int i;
  int id, numprocs;
  int n;
  char fn[256];
  int total_size;
  double start,end;

  if (argc<2) {
    printf("\n ...Wrong Number of arguments...  \n");
    printf("\n ...USAGE: mpirun -np [#ofprocessors] sorting [#of integers to be sorted]...  \n");
    exit(-1);
  }
  total_size = atol(argv[1]);

  A = (int*)malloc(total_size*sizeof(int));

  comm = MPI_COMM_WORLD;

  MPI_Init(&argc,&argv);

  /* a parallel array of total_size random integers */
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);
#ifdef DEBUG
  sprintf(fn,"debug%d", id);
  debug = fopen(fn,"w");
#endif

  n = total_size/numprocs;
  cons_array(A, id, total_size, n); /* construct initial array  */
  LOG("initial array\n");
  print_array(A,n);
  MPI_Barrier(MPI_COMM_WORLD);
  start = MPI_Wtime();
  hypercube_quicksort(A,n);
  MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();
  if (id==0) {
    printf("time=%f\n",end-start);
  }

  MPI_Finalize();

#ifdef DEBUG
  fclose(debug);
#endif

}
