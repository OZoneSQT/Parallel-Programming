## 9.17 
Consider the following parallel quicksort algorithm that takes advantage of the topology of a p -process hypercube connected parallel computer. Let n be the number of elements to be sorted and p = 2d be the number of processes in a d-dimensional hypercube. Each process is assigned a block of n /p elements, and the labels of the processes define the global order of the sorted sequence. The algorithm starts by selecting a pivot element, which is broadcast to all processes. Each process, upon receiving the pivot, partitions its local elements into two blocks, one with elements smaller than the pivot and one with elements larger than the pivot. Then the processes connected along the d th communication link exchange appropriate blocks so that one retains elements smaller than the pivot and the other retains elements larger than the pivot. Specifically, each process with a 0 in the d th bit (the most significant bit) position of the binary representation of its process label retains the smaller elements, and each process with a 1 in the d th bit retains the larger elements. After this step, each process in the (d - 1)-dimensional hypercube whose d th label bit is 0 will have elements smaller than the pivot, and each process in the other (d - 1)-dimensional hypercube will have elements larger than the pivot. This procedure is performed recursively in each subcube, splitting the subsequences further. After d such splits – one along each dimension – the sequence is sorted with respect to the global ordering imposed on the processes. This does not mean that the elements at each process are sorted. Therefore, each process sorts its local elements by using sequential quicksort. This hypercube formulation of quicksort is shown in Algorithm 9.9. The execution of the algorithm is illustrated in Figure 9.21 .

#### Figure 9.21. 
The execution of the hypercube formulation of quicksort for d = 3. The three splits – one along each communication link – are shown in (a), (b), and (c). The second column represents the partitioning of the n -element sequence into subcubes. The arrows between subcubes indicate the movement of larger elements. Each box is marked by the binary representation of the process labels in that subcube. A * denotes that all the binary combinations are included.

![image](https://user-images.githubusercontent.com/16867443/32344740-46dfcafa-bfde-11e7-9c44-a808714b1cf9.png)

#### Algorithm 9.9 A parallel formulation of quicksort on a d-dimensional hypercube. B is the n /p -element subsequence assigned to each process.
```
1.    procedure HYPERCUBE_QUICKSORT (B, n)
2.    begin
3.       id := process's label;
4.       for i := 1 to d do
5.       begin
6.          x := pivot;
7.          partition B into B1 and B2 such that B1 <= x < B2;
8.          if i-th bit is 0 then
9.          begin
10.            send B2 to the process along the i-th communication link;
11.            C := subsequence received along the i-th communication link;
12.            B := B1 UNION C ;
13.          endif
14.          else
15.             send B1 to the process along the i-th communication link;
16.             C := subsequence received along the i-th communication link;
17.             B := B2 UNION C ;
18.          endelse
19.       endfor
20.       sort B using sequential quicksort;
21.    end HYPERCUBE_QUICKSORT
```
Analyze the complexity of this hypercube-based parallel quicksort algorithm. Derive expressions for the parallel runtime, speedup, and efficiency. Perform this analysis assuming that the elements that were initially assigned at each process are distributed uniformly.
