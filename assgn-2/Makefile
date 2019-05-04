install:
	yes | cp -rf sched_ule.c /usr/src/sys/kern/sched_ule.c
	yes | cp -rf kern_switch.c /usr/src/sys/kern/kern_switch.c
	#yes | cp -rf runq.h /usr/src/sys/sys/runq.h
	cc -c benchmark.c
	cc -Wall -Werror -Wextra -Wshadow -O3 -pedantic -o benchmark benchmark.o

infer:
	make clean
	infer-capture -- make
	infer-analyze -- make

clean:
	rm -f benchmark test
	rm -fr infer-out

check_case:
	sysctl -n kern.sched.schedcase

case_1:
	sysctl -w kern.sched.schedcase=1
	
case_2:
	sysctl -w kern.sched.schedcase=2
	
case_3:
	sysctl -w kern.sched.schedcase=3
	
case_4:
	sysctl -w kern.sched.schedcase=4
	
	
