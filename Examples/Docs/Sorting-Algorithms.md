## Sorting Algorithms
The general asumption is that The sorted list is partitioned with the property that each partitioned list is sorted and each element in processor P<sub>*i*</sub>’s list is less than that in P<sub>*j*</sub>’s list if *i* < *j*.

A parallel compare-exchange operation. Processes P<sub>*i*</sub> and P<sub>*j*</sub> send their elements to each other. Process P<sub>*i*</sub> keeps min{a<sub>*i*</sub>, a<sub>*j*</sub>}, and P<sub>*j*</sub> keeps max{a<sub>*i*</sub>, a<sub>*j*</sub>}.

#### Bitonic Sort
A Bitonic sequence is list comprised of two halves, the first half is increasing the second half is decreasing and can start at any element in the given list.
![image](https://user-images.githubusercontent.com/16867443/33642726-3749c57e-da09-11e7-9fd3-71ceed716f19.png)
The comparator network that transforms an input sequence of 16 unordered numbers into a bitonic sequence
