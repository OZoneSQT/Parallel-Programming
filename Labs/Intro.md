# Intro
1. login to a to encs server...
2. ssh into username@apini.encs.concordia.ca
3. (first time run /locl/bin/ssh... may need to cd into dir)

4. make a [hello world](https://computing.llnl.gov/tutorials/mpi/samples/C/mpi_hello.c)
5. compile `mpicc hello-world.c -0 hello-world.run`
6. run it `mpirun -n 16 hello-world.run`

Sample output:
```bash
[apini] [/home/c/c_mcart/Documents] > mpirun -n 16 hello-world.run 
Hello from task 11 on apini-02.encs.concordia.ca!
Hello from task 12 on apini-02.encs.concordia.ca!
Hello from task 1 on apini-02.encs.concordia.ca!
Hello from task 6 on apini-02.encs.concordia.ca!
Hello from task 7 on apini-02.encs.concordia.ca!
Hello from task 8 on apini-02.encs.concordia.ca!
Hello from task 13 on apini-02.encs.concordia.ca!
Hello from task 15 on apini-02.encs.concordia.ca!
Hello from task 14 on apini-02.encs.concordia.ca!
Hello from task 2 on apini-02.encs.concordia.ca!
Hello from task 3 on apini-02.encs.concordia.ca!
Hello from task 4 on apini-02.encs.concordia.ca!
Hello from task 9 on apini-02.encs.concordia.ca!
Hello from task 5 on apini-02.encs.concordia.ca!
Hello from task 10 on apini-02.encs.concordia.ca!
Hello from task 0 on apini-02.encs.concordia.ca!
MASTER: Number of MPI tasks is: 16
```
