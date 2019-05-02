install:
	yes | cp -rf sched_ule.c /usr/src/sys/kern/sched_ule.c
	yes | cp -rf kern_switch.c /usr/src/sys/kern/kern_switch.c
	yes | cp -rf runq.h /usr/src/sys/sys/runq.h

case0:
	sysctl -w sched_ule.schedcase 0

case_1:
	sysctl -w sched_ule.schedcase 1

case2:
	sysctl -w sched_ule.schedcase 2

case3:
	sysctl -w sched_ule.schedcase 3
	
benchmark:
	cc -c benchmark.c
	cc -o benchmark benchmark.o
	
	
