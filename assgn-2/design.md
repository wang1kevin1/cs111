# Design Overview 

## Team: W's Only

* **Kevin Wang** - case switching, splatter scheduling
* **Malcolm Neill** - splatter scheduling
* **Nicolette Miller** - priority queue
* **Edmund Yu** - benchmark

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

```schedcase``` = 3

## Design

**Switching Cases**
In order to be able to easily implement all four cases, we will use a global value ```schedcase```. This static int value can be updated using the FreeBSD sysctl(9). This will allow us to switch between scheduling cases during runtime while the kernel is loaded.

**Ignore Kernel Threads**
Note that a kernel thread has priority value ```td->td_priority``` of 0 to 47 and 80 to 119 -- inclusive. 

**Random Number Generator**
In order to create a random number generator, we

**Assigning a Random Run Queue**

If ```schedcase``` is equal to 3 or 4,

**Priority Queues**

If ```schedcase``` is equal to 2 or 4, 

**Benchmark**

In order to test thread scheduling, our benchmark will create a large amount of processes using a fork bomb.
We will also run fibonacci.

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


