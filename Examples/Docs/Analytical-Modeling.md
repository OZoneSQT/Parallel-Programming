## Analytical Modeling
A sequential program is evaluated based on its runtime, asymptotic runtime as a function of input size, which does not vary bsed on the serial platform. The parallel program depends heavily on the underlying platform because of the number of processesors and communication parameters; algorithms are analyzed in the context of the underlying platform.

#### Sources of Overhead
- Interprocess interactions: Processors working on any non-trivial parallel problem will need to talk to each other
- Idling: Processes may idle because of load imbalance, synchronization, or serial components
- Excess Computation: This is computation not performed by the serial version due to parellelization complexity or duplicated computations

#### Performance metrics
**Term** | **Meaning** | **Equation**
---|---|---
T<sub>all</sub> | Runtime of any program |
T<sub>s</sub> | Runtime of a serial program |
T<sub>p</sub> | Runtime of a parallel program |
T<sub>o</sub> | Total overhead (non-useful work) of a program  | T<sub>o</sub> = *p*T<sub>p</sub> - T<sub>s</sub>
*S* | Speedup - Ration of time require on a single CPU to the time taken by a parallel computer | T<sub>s</sub> / T<sub>p</sub>
*E* | Efficiency - Fraction of time CPU was usefully employed | *E* = *S* / *p*
C | Cost is the sum of time spent by all CPUs; optimal when *E* = O(1) | C = *p*T<sub>p</sub>

#### Scalability
To determine the scalability of a parallel system, we need to find the rate at which problem size must increase with the number of CPUs increasing to keep the efficiency fixed; the lower this rate the more scalable the system is.

**Term** | **Meaning** | **Equation**
---|---|---
K | constant depending on the efficiency to be maintained | K = *E* / (1-*E*)
*W* | problem size or workload | *W* = K T<sub>o</sub>(*W*,*p*)

T<sub>o</sub>(*W*,*p*) is the function for _Total Overhead_ at the given problem size for a number of processors.

#### Amdahl's Law
**Term** | **Meaning** | **Equation**
---|---|---
T | execution time of the whole task _before_ the improvement of the resources of the system | T = (1-*p*)T + *p*T
T(*S*) | execution time of the whole task **after** the improvement of the resources of the system | T(*S*) = (1-*p*)T + (*p* / *S*)T
*S*<sub>latency</sub>(*S*) | theoretical speedup in latency of the execution of the whole task at fixed workload | *S*<sub>latency</sub> = 1 / (1 - *p* + (*p* / *S*))
