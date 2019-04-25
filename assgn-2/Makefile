install:
	yes | cp -rf sched_ule.c /usr/src/sys/kern/sched_ule.c
	yes | cp -rf kern_switch.c /usr/src/sys/kern/kern_switch.c
	yes | cp -rf runq.h /usr/src/sys/sys/runq.h
	
benchmark:
	cc -c benchmark.c
	cc -o benchmark benchmark.o
	
	
