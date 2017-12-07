## Transformations

#### Loop Rerorering
Change the relative order of execution of the iterations of a loop nest(s) to expose parallelism and improve memory locality. This can be done by parallelizing the outter or inner loops and may involve changing the direction of the iteration.

###### Loop Interchange
Loop interchange exchanges the position of two loops moving one of the outer loops to the innermost position. This is done to impreove vectorization or parallel performance.

###### Loop Reversal
Reversal changes the direction in which the loop traverses its iteration range. It is often used in conjunction with other iteration space reordering transformations because it changes the dependence vectors.

###### Cycle Shrinking
When a loop has dependence that prevent it from being executed in parallel the compiler may still be able to expose some parallelism if the dependence distance is greater than one will convert a serial loop into an outer serial loop and an inner parallel loop.
