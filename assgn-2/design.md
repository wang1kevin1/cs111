# Design Overview 

## Team: W's Only

* **Kevin Wang** - design, case switching, splatter scheduling
* **Malcolm Neill** - design, rng, splatter scheduling
* **Nicolette Miller** - design, priority queue
* **Edmund Yu** - design, benchmark, benchmark analysis

## Scheduling Cases

### Case 1: ULE scheduler and FIFO queues.

Base implementation of the FreeBSD system. No changes needed. Note that the original code must be preserved and 
any changes will require if/else statements to check if a thread is user or kernel, as well as what scheduling case is active.

```schedcase``` = 1

### Case 2: ULE scheduler with priority queues.

ULE scheduling assigns the process threads to their respoective run queues. 
The threads will be inserted into the FIFO queues by priority (simulating a priority queue). 

```schedcase``` = 2

### Case 3: Splatter scheduler and FIFO queues.

User process threads will be assigned to a random FIFO run queue.

```schedcase``` = 3

### Case 4: Splatter scheduler and priority queues.

User process threads will be assigned to a random run queue.
The threads will be inserted into the FIFO queues by priority (simulating a priority queue).

```schedcase``` = 4

## Design

**Switching Cases**

In order to be able to easily implement all four cases, we will use a global value ```schedcase```. This static int value can be updated using the FreeBSD sysctl(9). This will allow us to switch between scheduling cases during runtime while the kernel is loaded.

**Ignore Kernel Threads**

Note that a kernel thread has priority value ```td->td_priority``` of 0 to 47 and 80 to 119 -- inclusive. 
We will use a boolean value ```isKernel``` which will say if a priority falls within the boundaries.

**Random Number Generator**

In order to create a random number generator, we

**Assigning a Random Run Queue**

If ```schedcase``` is equal to 3 or 4, we will use a random priority provided by the RNG function.
When assigning a random run queue, FreeBSD typically uses ```td_priority``` however we can set the buffer value ```pri```
to a random one.

For kernel threads and other cases, the buffer value ```pri``` will still be equal to ```td->td_priority```.

Assigning of queues will be the same for all cases, with varying results due to ```pri```.

**Priority Queues**

If ```schedcase``` is equal to 2 or 4, we will add a thread to its assigned queue, ordered by priority. 
While the run queue will still use FreeBSD's FIFO, it will simulate a priority queue. 
Because we will be inserting items throughout the run queue, we will be using linked-lists.
While we should've used heaps -- O(lg n) -- and implemented an extractMin for choosing a thread to run , 
linked-lists -- O(n) was easier with the library TAILQ procedures)

If a run queue is empty, we will insert the thread at the head. Otherwise, 
we will cycle through the run queue and compare our threads actual priority against 
the temporary selected run queue thread's priority.

When comparing, if our thread has a lower priority (bigger value), we will either continue to the next item
or -- if at the end of the run queue -- insert it after. If our thread has a higher priority (smaller value), 
we will insert it before the temporarily selected thread.

**Benchmark**

In order to test thread scheduling, our benchmark will run a fork bomb, fibonacci, and test threading.

## Kernel Modifications

### Data 

**kern_switch.c**
```
static int schedcase	// used to determine which kernel case is being used
```

### Functions

**kern_switch.c**

```
// rng function
```

```
runq_add
```

```
runq_add_pri
```

```
// nikki's insert by priority function
```

## Benchmark Analysis





# Sources
* "Design and Implementation of the FreeBSD Operating Systems"
* Piazza
* http://www.leidinger.net/FreeBSD/dox/kern/html/
* https://wiki.freebsd.org/AndriyGapon/AvgThreadPriorityRanges
* https://www.freebsd.org/doc/en/books/handbook/kernelconfig.html
* https://www.freebsd.org/doc/handbook/kernelconfig/building.html


