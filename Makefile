CFLAGS=-Wall -Werror -Wextra -Wshadow -O3 -pedantic
CC=cc
EXEC = performance
LIB= -lpthread

make     :
	 c++ ${CFLAGS} -o performance performance.cc ${LIB}

infer    :
	 make clean; infer-capture -- make; infer-analyze -- make

run      :
	 time ./${EXEC} > test

clean    :
	 rm -f performance test
	 rm -fr infer-out
