install:
	yes | cp -rf sched_ule.c /usr/src/sys/kern/sched_ule.c
	#yes | cp -rf kern_switch.c /usr/src/sys/kern/kern_switch.c
	#yes | cp -rf runq.h /usr/src/sys/sys/runq.h

check_case:
	sysctl -n kern.sched.schedcase

case1:
	sysctl -w kern.sched.schedcase=1
	
case2:
	sysctl -w kern.sched.schedcase=2
	
case3:
	sysctl -w kern.sched.schedcase=3
	
case4:
	sysctl -w kern.sched.schedcase=4
	
benchmark:
	cc -c benchmark.c
	cc -o benchmark benchmark.o
	
	
