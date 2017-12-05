## Parallel Platforms
Currently systems are limited by processing and memory. To improve computation speed piplining is used, like a car manufature, but this is limited to the slowest stage. In programs logical jumps are very common meaning the pipe isnt intake for long. Memory access has a high overhead cost, most CPUs come with cache to elivate this but the latency and bandwidth are still factors.

A parallel platform is comprised of
- Control Structure: specified concurrency (various from instruction to processes depending on model)
- Communication Model: interaction between concurrent subtasks

#### Control Structure
*[S/M]I[S/M]D --> S = Single; M = Multiple;*

Two main workflows are SIMD and MIMD. SIMD has single control unit sending the same instruction to multiple processors each with a data set whereas in MIMD processors each have a control unit. SIMD is best for regular structures (is Image Processing), usualy implemented with a _Activity Mask_  to select which CPU partakes in a computation. A spin off of MIMD is SPMD (P = Program;) this is how OpenMPI is done. SIMD units are far more complex in terms of design despite using less hardware compared to SPMD which can be donw with off the self parts.

#### Communication Model
These fall into tow groups...
- Shared Adress Space (often seen in multi CPU systems)
- Message Passing (usually multi computer networking)

Shared address space systems and have uniform or non-uniform memory access. Uniformity is defiend by all memory accesses taking the same time. A uniform architecture would have only global RAM (maybe local caches) whereas on a non-uniform architecture would have only local RAM. These variation are tailored to different types of problems. Message Passing requires only a network again this is how OpenMPI works.

#### Interconnections
These networks can be static (links between each node) or dynamic (nodes communicate through switches which route accordingly). Switches have a cost that is the square of the number of ports. This has lead to many network topoligies being created..
- bus (my workstation)
- crossbar (grid of processors to memory banks)
- Multistaging
- Completely connected (all to all)
- Star connected (all to one)
- Linear Array (to neighbor)
- Meshes (to neighbor)
- Hypercubes (`log p` connections with max `log p` distance)
- Tree-Base (maybe be static or dynamic)

Evaluation of Networks
- Diameter: The distance between the farthest two nodes
- Bisection Width: The minimum number of wires you must cut to divide the network into two equal parts
- Cost: The number of links or switches
