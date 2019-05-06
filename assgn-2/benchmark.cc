// benchmark.c
// Used for testing the scheduler
// Assgn-2 Darrell Long Spring 2019
//
// Kevin Wang
// Malcolm Neill
// Nikki Miller
// Edmund Yu
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>
// manpage for getpid() no errors

// Create many child processes
void forkbomb(int current, int limit);
long fib(int digits);
void squared(int in);


//Prints out current thread ID
//https://thispointer.com/c11-how-to-create-vector-of-thread-objects/
void threadID() {
  int counter = 0;
  for (int i =0; i < 10000; i++) {
    counter++;
  }
  if (counter > 998) {
  std::cout << "Current thread ID : " << std::this_thread::get_id() << "\n";
  }
}

// stackoverflow.com/questions/346613
// creates multiple forks until the limit, doing small calculations so the compiler does not optimize it away. 
void forkbomb(int current, int limit) {
  pid_t cpid;
  printf("current fork: %d \n ", current);
  //exit when current hits the limit
  if (current == limit)
    return;

  // create new fork, run fib(), recursively increment current
  if (current < limit) {
    if ((cpid = fork()) < 0) {
      int modded = current % 5;
      fib(modded);
    }
    current++;
    forkbomb(current, limit);
  }
}

// Print out Fibonacci numbers the slow way
long fib(int digits) {
 
  if (digits < 2) {
    printf("fib(%d) = 1\n", digits);
    return 1;
  }
  long f = fib(digits - 1) + fib(digits - 2);
  printf("fib(%d) = %ld\n", digits, f);
  return f;
}

int main() {
  pid_t pid, wpid;

  int statloc = 0;
  // stackoverflow.com/questions/10509660
  // stackoverflow.com/questions/46105827
  // stackoverflow.com/questions/19461744
  // stackoverflow.com/questions/5248915
  struct rusage use_end;
  struct timeval end;
  
  //makes 3 forks to run a forkbomb, a time consuming function, and threads
  for (int i = 0; i < 3; i++) {
    pid = fork();
    if (pid < 0) {
      perror("fork() failed");
    }
    if (pid == 0) {
      printf("Currently child  with id:%d, loop%d\n", getpid(), i);

      if (i == 0) {
        printf("loop 1 \n");
        forkbomb(0, 10);
      }
      if (i == 1) {
        printf("loop 2 \n");
        printf("Fib of 30 is: %ld\n", fib(30));
      }
      // help from piazza @347
      if (i == 2) {
        printf("loop 3\n");
        std::vector<std::thread> threads;
        for (int j = 0; j < 32; j++) {
          threads.push_back(std::thread(threadID));
        }
        for (auto &thread : threads) {
          thread.join();
        }
      }

      exit(0);

    } else {
      // waits for children to finish
      printf("Currently parent with id:%d, loop%d\n", getpid(), i);
      pid = waitpid(pid, &statloc, 0);

      while ((wpid = wait(&statloc)) > 0)
        ;
      
      // prints out time usage of user and kernel
      getrusage(RUSAGE_CHILDREN, &use_end);
      end = use_end.ru_stime;

      printf("Currently elapsed stime: %lds, %ldmicro\n", end.tv_sec, end.tv_usec);

      end = use_end.ru_utime;
      printf("Currently elapsed utime: %lds, %ldmicro\n", end.tv_sec, end.tv_usec);
    }
  }

  printf("end loop\n");

  return 0;
}
