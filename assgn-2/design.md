# Design Overview 

## Todo MAKE SURE TO ADD DESIGN DOC STUFF WHILE YOU’RE CODING YOUR PART
* Benchmark script + dmesg/log output (EY)
* Shell script and ifndef for switching cases/loading kernel mode (kw)
* Splatter scheduler (random generator w/data structures) (MN)
* Priority queue (priority queue, use heap or linked-list and why?) (Nikki) 

## Team: W's Only

* **Kevin Wang**
* **Malcolm Neill**
* **Nicolette Miller**
* **Edmund Yu**

## Splatter Scheduler

### Case 1: ULE scheduler and FIFO queues.

Base implementation of the FreeBSD system. No changes needed. Note that the original code must be preserved and any changes will require if/else/switch boolean checks.

### Case 2: ULE scheduler with priority queues.

User threads are assigned to queues based on priority. Manipulation function, see TAILQ.

### Case 3: Splatter scheduler and FIFO queues.

User process threads given a random value and added to a FIFO queue based on lottery.
(how to do random? - built in w/freebsd or separate alg?)

### Case 4: Splatter scheduler and priority queues.

User process threads are assigned a random priority value and added to a priority queue.

### Switching Cases
In order to be able to easily implement all four cases, we will add a global value ```sched_case``` to sched_ule.c. This static int value can be updated using the FreeBSD sysctl(9). To allow easy switching, we can specify which scheduling case we want while the kernel is loaded.

## Kernel Modifications

### Data 
make sure to add lines of code (can be pseudo) added/changed and description of why and what it does.

**sched_ule.c**
```
static int schedcase	// used to determine which kernel case is being used
```

### Functions 


## Benchmark Analysis


# Sources
* "Design and Implementation of the FreeBSD Operating Systems"
* Piazza
* http://www.leidinger.net/FreeBSD/dox/kern/html/
* https://wiki.freebsd.org/AndriyGapon/AvgThreadPriorityRanges
* https://www.freebsd.org/doc/en/books/handbook/kernelconfig.html
* https://www.freebsd.org/doc/handbook/kernelconfig/building.html


