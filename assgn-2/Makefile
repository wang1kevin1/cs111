install:
	yes | cp -rf kern_switch.c /usr/src/sys/kern/kern_switch.c
	c++ -c benchmark.cc
	c++ -Wall -Werror -Wextra -Wshadow -O3 -pedantic -o benchmark benchmark.o -lpthread

uninstall:
	yes | cp -rf /unedited/kern/kern_switch.c /usr/src/sys/kern/kern_switch.c
	rm -f benchmark benchmark.o benchmark_log

benchmark:
	./benchmark > benchmark_log

stats:
	time ./benchmark

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
	
	
