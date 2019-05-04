install:
	yes | cp -rf kern_switch.c /usr/src/sys/kern/kern_switch.c
	cc -c benchmark.c
	cc -Wall -Werror -Wextra -Wshadow -O3 -pedantic -o benchmark benchmark.o

uninstall:
	yes | cp -rf /unedited/kern/kern_switch.c /usr/src/sys/kern/kern_switch.c
	rm -f benchmark benchmark.o benchmark_results

benchmark:
	./benchmark > benchmark_log

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
	
	
