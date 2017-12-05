## Principles of Parallel Algorithm Design
Developing a parallel algorithm begins with decompose the problem (make concurrent tasks). The depth of the decomposition determines the granularity of the problem. The more tasks the finer the granularity of the problem (most fine grain being 1 task to 1 CPU). The number of tasks that can be executed in parallel is the _degree of concurrency_ of a decomposition. The longest path in the 'task dependency graphy' is the _critical path_ and it determines the minumum runtime of the program.

The parallel performance is often limited by the overhead which arise from the granularity of a decomposition, in particular cummunication over head. The finer the decomposition the associated overhead, a ratio of useful work assocaited with a task, increases. Since performance is tied to the underlying structure tasks need to be mapped to processors such that the minimum parallel execution time can be reached.

#### Decomposition Techniques
- recursive decomposition: good with a divide-and-conquer strategy; break down problem until desired size and concat results
- data decomposition
  - output data: each output can be independently calculated, partiton sections to be computed.
  - input data: output based on input but is unknown prior (ie finding min in a list)
- exploratory decomposition: number of processes grows as the program runs until the goal node is reached
- speculative decomposition: List of known tasks which may be independant, each is executed, resultign tasks add to list of modify it

#### Tasks
Generateion may be static where tasks are know priror to runtime, operation are performed on a regularly structures set; these are recursive or data decomposed. Dynamic tasks are generates during runtime; these are typically from exploratory or speculative decompositions. Task size may be uniform, all having the same size. non-uniform tasks size may or may not be estimated prior to runtime. The size of the data set of the task may be viewed as large or small in context of the task size itself. Task interaction mya again be static or dynamic. Regular interaction can be exploit a pattern for efficiency whereas irregular interaction lack a defined topology.

#### Mapping
Mappings must minimize overheads, communication and idling, these overheads often represents contradicting objectives. Static mapping based on data partition, task graph or a combination. Dynamic may be centralized (masters and slaves) were work is requested from the dispatcher or it can distributed when workers transfer work between each other. Minimizing interaction for max locality (reuse intermdiate data), reduce volume of interactions, reduce frequence, reduce contentions by replicating data. Use non-blocking and prefetching to hide latency, multicast over unicast over lap interactions.

#### Models
- Data Parallel Model: Tasks are statically (or semi-statically) mapped to processes and each task performs similar operations on different data.
- Task Graph Model: Starting from a task dependency graph, the interrelationships among the tasks are utilized to promote locality or to reduce interaction costs.
- Master-Slave Model: One or more processes generate work and allocate it to worker processes. This allocation may be static or dynamic.
- Pipeline / Producer-Comsumer Model: A stream of data is passed through a succession of processes, each of which perform some task on it.
- Hybrid Models: A hybrid model may be composed either of multiple models applied hierarchically or multiple models applied sequentially to different phases of a parallel algorithm.
