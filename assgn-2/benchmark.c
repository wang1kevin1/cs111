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
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// manpage for getpid() no errors

// Create many child processes
void forkbomb(int current, int limit);
long fib(int digits);


//stackoverflow.com/questions/346613
void forkbomb(int current, int limit) { 
  pid_t cpid; 
  printf("current fork: %d \n ", current);
  if (current == limit) return;
  if (current < limit) {
    if ((cpid = fork()) == 0) {
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

  for (int i = 0; i < 3; i++) {
    pid = fork();
    if (pid < 0) {
      perror("fork() failed");
    }
    if (pid == 0) {
      printf("Currently child  with id:%d, loop%d\n", getpid(), i);

      if (i == 0) {
        forkbomb(0, 10);
      }
      if (i == 1) {
        printf("loop 2 \n");
        printf("Fib of 10 is: %ld\n", fib(10));
      }

      if (i == 2) {
        printf("loop 3\n");
      }

      exit(0);
      //   getrusage(RUSAGE_CHILDREN, &usage);
      //   end = usage.ru_stime;
      //   printf("End: %lds, %ldmicro\n", end.tv_sec, end.tv_usec);
    } else {
      printf("Currently parent with id:%d, loop%d\n", getpid(), i);
      pid = waitpid(pid, &statloc, 0);
      /*
      if (i == 0) {
        getrusage(RUSAGE_CHILDREN, &use_l1);
        l1 = use_l1.ru_stime;
        printf("L1 stime: %lds, %ldmicro\n", l1.tv_sec, l1.tv_usec);

        l1 = use_l1.ru_utime;
        printf("L1 utime: %lds, %ldmicro\n", l1.tv_sec, l1.tv_usec);
      }
      if (i == 1) {
        getrusage(RUSAGE_CHILDREN, &use_l2);
        l2 = use_l2.ru_stime;
        printf("L2 stime: %lds, %ldmicro\n", l2.tv_sec, l2.tv_usec);

        l2 = use_l2.ru_utime;
        printf("L2 utime: %lds, %ldmicro\n", l2.tv_sec, l2.tv_usec);
      }

      if (i == 2) {
        getrusage(RUSAGE_CHILDREN, &use_l3);

        l3 = use_l3.ru_stime;
        printf("L3 stime: %lds, %ldmicro\n", l3.tv_sec, l3.tv_usec);

        l3 = use_l3.ru_utime;
        printf("L3 utime: %lds, %ldmicro\n", l3.tv_sec, l3.tv_usec);
      }
      */
      while ((wpid = wait(&statloc)) > 0)
        ;

      getrusage(RUSAGE_CHILDREN, &use_end);
      end = use_end.ru_stime;
  
      printf("end stime: %lds, %ldmicro\n", end.tv_sec, end.tv_usec);

      end = use_end.ru_utime;
      printf("end utime: %lds, %ldmicro\n", end.tv_sec, end.tv_usec);
    }
  }
  // while((wpid = wait(&statloc)) >= 0);

  printf("end loop\n");

  getrusage(RUSAGE_SELF, &use_end);
  end = use_end.ru_stime;
  printf("end stime: %lds, %ldmicro\n", end.tv_sec, end.tv_usec);

  end = use_end.ru_utime;
  printf("end utime: %lds, %ldmicro\n", end.tv_sec, end.tv_usec);

  // printf("time taken = %f \n", time);
  //  printf("Start: %lds, %ldmicro\n", start.tv_sec, start.tv_usec);
  // printf("End: %lds, %ldmicro\n", end.tv_sec, end.tv_usec);

  return 0;
}
