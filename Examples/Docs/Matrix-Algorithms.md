## Matrix Algorithms
Very easy to data decompose due to regular structure. _This section may need to be reviewed_

#### Matrix by Vector Multiplication
###### Row-wise 1-D Partition
Each CPU gets a row of the matrix and element in each vector. each CPU does its work multiple each element on the row but the input vector value. at the end an all-to-all broadcast is required such that all CPUs have all the values in the output vector. When `p = n`, T<sub>p</sub> = T<sub>comp</sub> + T<sub>comm</sub> == &#x3f4;(n) + &#x3f4;(n) === &#x3f4;(n). When `p < n` the matrix is partitioned into n/p rows to each CPU, it calculates each row's output. this is follows by an all-to-all broardcast amound p CPUs and a n/p message size. an additional n/p dot products will need to be calculated by each CPU. T<sub>p</sub> = T<sub>comp</sub> + T<sub>comm</sub> + T<sub>dot</sub> == O(n/p * n) + O(log p) + O(n). This is cost-optimal.

###### 2-D Partition
Comprised of 3 different communication phases each taking &#x3f4;(log n) which gives us our parallel time. the cost is &#x3f4;(n<sup>2</sup> log n) which is not optimal. The isometric function is O(p log<sup>2</sup>p).

#### Matrix by Matrix Multiplication
###### DNS Algorithm
Uses 3-D partitioning. A and B come in two orthogonal faces and result C comes out the other orthogonal face. using an n<sup>3</sup> mesh, A defines the column and B the rows; each CPU does a single addition/multiplication. This is followed by an accumulation along the C dimension. Since each add-multiply takes constant time and accumulation and broadcast takes log n time, the total runtime is log n. It can be made cost optimal by using n/ log n processors on the C face. 
![image](https://user-images.githubusercontent.com/16867443/33640622-94d8dc66-d9ff-11e7-829e-88adcb69b526.png)
When using fewer than n<sup>3</sup> CPUs, The parallel time is approximated by T<sub>p</sub> = n<sup>3</sup> / p + log p + ( n<sup>2</sup> / p<sup>2/3</sup>)log p. The isoefficiency function is &#x3f4;(p(log p<sup>3</sup>))
