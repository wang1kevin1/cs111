/* Sun Apr 21 12:26:25 PDT 2019 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define NO_ARGS       "No arguments on line!\n"
#define EXIT_MSG      "Exiting...\n"

extern char ** get_args();

/* Print args and their indexes */
void print_args(char **args) {
  for (int i = 0; args[i] != NULL; i++) {
      printf("Argument %d: %s\n", i, args[i]);
  }
  return;
}

/* Print args and their indexes */
void print_args1(char **args) {
  printf("args: [ ");
  for (int i = 0; args[i] != NULL; i++) {
      printf("\"%s\" ", args[i]);
  }
  printf("]\n");
  return;
}

/* System Command Execution ( ls grep cat ... ) */
void exec_command(char **args) {
  pid_t pid;
  int wstatus;

  pid = fork();                                 // fork process

  if (pid == -1) {                              // no child process created
      perror("ERROR [exec_command; fork()]");
      return;
  } else if (pid == 0) {                        // child process
      if (execvp(args[0], args) == -1) {
        // execvp() only returns when error overwriting process image
        perror("ERROR [exec_command; execvp()]");
        exit(1);
      }
  } else {                                      // parent process
      // wait until child process changes state
      wait(&wstatus);
      return;
  }
}

/* Input Redirection & File Ops ( < ) */
void input_redirect(char **args, int i) {
  pid_t pid;
  int infile;
  int wstatus;

  // mark termination of pre '<|>' command
  args[i] = NULL;

  pid = fork();                                 // fork process

  if (pid == -1) {                              // no child process created
      perror("ERROR [input_redirect; fork()]");
      return;
  } else if (pid == 0) {                        // child process  
      // get descriptor for opened read-only input file
     infile = open(args[i+1], O_RDONLY);
      if (infile == -1) {
        perror("ERROR [input_redirect; open()]");
        exit(1);
      }

      // duplicates open file to descriptor 0 (originally stdin)
      if (dup2(infile, 0) == -1) {
        perror("ERROR [input_redirect; dup2()]");
        exit(1);
      }

      close(infile); // close file

      // execute
      if (execvp(args[0], args) == -1) {
        // execvp() only returns when error overwriting process image
        perror("ERROR [input_redirect; execvp()]");
        exit(1);
      }

  } else {                                      // parent process
      // wait until child process changes state
      wait(&wstatus);
      return;
  }
} 

/* Output Redirection & File Ops ( > ) */
void output_redirect(char **args, int i) {
  pid_t pid;
  int outfile;
  int wstatus;

  // mark termination of pre '>' command
  args[i] = NULL; 

  pid = fork();                                 // fork process

  if (pid == -1) {                              // no child process created
      perror("ERROR [output_redirect; fork()]");
      return;
  } else if (pid == 0) {                        // child process  
      // get descriptor for created (-rw-r--r--) or opened file for output
      outfile = open(args[i+1], O_RDWR | O_CREAT | O_TRUNC, 
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
      if (outfile == -1) {
        perror("ERROR [output_redirect; open()]");
        exit(1);
      }

      // duplicates open file to descriptor 1 (originally stdout)
      if (dup2(outfile, 1) == -1) {
        perror("ERROR [output_redirect; dup2()]");
        exit(1);
      }

      close(outfile); // close file

      // execute
      if (execvp(args[0], args) == -1) {
        // execvp() only returns when error overwriting process image
        perror("ERROR [output_redirect; execvp()]");
        exit(1);
      }

  } else {                                      // parent process
      // wait until child process changes state
      wait(&wstatus);
      return;
  }
}

/* Output Redirection & File Append ( >> ) */
void append_redirect(char **args, int i) {
  pid_t pid;
  int appfile;
  int wstatus;

  // mark termination of pre '>>' command
  args[i] = NULL; 

  pid = fork();                                 // fork process

  if (pid == -1) {                              // no child process created
      perror("ERROR [append_redirect; fork()]");
      return;
  } else if (pid == 0) {                        // child process  
      // get descriptor for opened or created (-rw-r--r--) file for appending
      appfile = open(args[i+1], O_RDWR | O_CREAT | O_APPEND, 
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
      if (appfile == -1) {
        perror("ERROR [append_redirect; open()]");
        exit(1);
      }

      // duplicates open file to descriptor 1 (originally stdout)
      if (dup2(appfile, 1) == -1) {
        perror("ERROR [append_redirect; dup2()]");
        exit(1);
      }

      close(appfile); // close file

      // execute
      if (execvp(args[0], args) == -1) {
        // execvp() only returns when error overwriting process image
        perror("ERROR [append_redirect; execvp()]");
        exit(1);
      }

  } else {                                      // parent process
      // wait until child process changes state
      wait(&wstatus);
      return;
  }
}

/* Single Pipe Commands ( | ) */
void pipes(char **args, int i) {
  pid_t pid;
  int wstatus;
  int pipe_chain[2]; // holds filedes of read end [0] and write end [1]

  char **   pre_args = &args[0];    // args before '|'
  char **   post_args = &args[i+1]; // args after '|'

  // mark termination of pre '|' command
  args[i] = NULL;

  if (pipe(pipe_chain) == -1) {                   // create pipe
    perror("ERROR [pipes; pipe()]");
    exit(1);
  }                               

  pid = fork();                                   // fork pre process

  if (pid == -1) {                                // no child process created
      perror("ERROR [pipes; fork()_1]");
      return;
  } else if (pid == 0) {                          // child process  
      // duplicates write end of pipe to descriptor 1 (originally stdout)
      if (dup2(pipe_chain[1], 1) == -1) {
        perror("ERROR [pipes; dup2()_1]");
        exit(1);
      }

      // close child process pipe
      close(pipe_chain[0]);
      close(pipe_chain[1]);

      // execute pre post '|' process
      if (execvp(args[0], pre_args) == -1) {
        // execvp() only returns when error overwriting process image
        perror("ERROR [pipes; execvp()_1]");
        exit(1);
      }
  } else {                                        // parent process
      pid = fork();                               // fork post process

      if (pid == -1) {                            // no child process created
          perror("ERROR [pipes; fork()_2]");
          return;
      } else if (pid == 0) {                      // secondary child process  
          // duplicates read end of pipe to descriptor 0 (originally stdin)
          if (dup2(pipe_chain[0], 0) == -1) {
            perror("ERROR [pipes; dup2()_2]");
            exit(1);
          }
          
          // close secondary child process pipe
          close(pipe_chain[0]);
          close(pipe_chain[1]);

          // execute post '|' process
          if (execvp(args[i+1], post_args) == -1) {
            // execvp() only returns when error overwriting process image
            perror("ERROR [pipes; execvp()_2]");
            exit(1);
          }
      } else {                                    // rest of parent process
        
        // close parent process pipe
        close(pipe_chain[0]);
        close(pipe_chain[1]);
        
        // wait for both processes to change states
        wait(&wstatus);
        wait(&wstatus);
        return;
      }
  }
}

/* Output Error Redirection & File Ops ( >& ) */
void output_error(char **args, int i) {
  pid_t pid;
  int outfile;
  int wstatus;

  // mark termination of pre '>' command
  args[i] = NULL; 

  pid = fork();                                 // fork process

  if (pid == -1) {                              // no child process created
      perror("ERROR [output_error; fork()]");
      return;
  } else if (pid == 0) {                        // child process  
      // get descriptor for created (-rw-r--r--) or opened file for output
      outfile = open(args[i+1], O_RDWR | O_CREAT | O_TRUNC,
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
      if (outfile == -1) {
        perror("ERROR output_error; open()]");
        exit(1);
      }

      // duplicates open file to descriptor 1 (originally stdout)
      if (dup2(outfile, 1) == -1) {
        perror("ERROR [output_error; dup2()_1]");
        exit(1);
      }

      // duplicates open file to descriptor 2 (originally stderr)
      if (dup2(outfile, 2) == -1) {
        perror("ERROR [output_error; dup2()_2]");
        exit(1);
      }

      close(outfile); // close file

      // execute
      if (execvp(args[0], args) == -1) {
        // execvp() only returns when error overwriting process image
        perror("ERROR [output_error; execvp()]");
        exit(1);
      }

  } else {                                      // parent process
      // wait until child process changes state
      wait(&wstatus);
      return;
  }
}

/* Append Error Redirection & File Ops ( >>& ) */
void append_error(char **args, int i) {
  pid_t pid;
  int appfile;
  int wstatus;

  // mark termination of pre '>>' command
  args[i] = NULL; 

  pid = fork();                                 // fork process

  if (pid == -1) {                              // no child process created
      perror("ERROR [append_error; fork()]");
      return;
  } else if (pid == 0) {                        // child process  
      // get descriptor for opened or created (-rw-r--r--) file for appending
      appfile = open(args[i+1], O_RDWR | O_CREAT | O_APPEND,
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
      if (appfile == -1) {
        perror("ERROR [append_error; open()]");
        exit(1);
      }

      // duplicates open file to descriptor 1 (originally stdout)
      if (dup2(appfile, 1) == -1) {
        perror("ERROR [append_error; dup2()_1]");
        exit(1);
      }

      // duplicates open file to descriptor 2 (originally stderr)
      if (dup2(appfile, 2) == -1) {
        perror("ERROR [append_error; dup2()_2]");
        exit(1);
      }

      close(appfile); // close file

      // execute
      if (execvp(args[0], args) == -1) {
        // execvp() only returns when error overwriting process image
        perror("ERROR [append_error; execvp()]");
        exit(1);
      }

  } else {                                      // parent process
      // wait until child process changes state
      wait(&wstatus);
      return;
  }
}

/* Single Pipe Error Commands ( |& ) */
void pipe_error(char **args, int i) {
  pid_t pid;
  int wstatus;
  int pipe_chain[2];                // holds filedes of read end [0] and write end [1]

  char **   pre_args = &args[0];    // args before '|'
  char **   post_args = &args[i+1]; // args after '|'

  // mark termination of pre '|' command
  args[i] = NULL;

  if (pipe(pipe_chain) == -1) {                   // create pipe
    perror("ERROR [pipe_error; pipe()]");
    exit(1);
  }                               

  pid = fork();                                   // fork pre process

  if (pid == -1) {                                // no child process created
      perror("ERROR [pipe_error; fork()_1]");
      return;
  } else if (pid == 0) {                          // child process  
      // duplicates write end of pipe to descriptor 1 (originally stdout)
      if (dup2(pipe_chain[1], 1) == -1) {
        perror("ERROR [pipe_error; dup2()_1.1]");
        exit(1);
      }

      // duplicates write end of pipe to descriptor 2 (originally stderr)
      if (dup2(pipe_chain[1], 2) == -1) {
        perror("ERROR [pipe_error; dup2()_1.2]");
        exit(1);
      }

      // close child process pipe
      close(pipe_chain[0]);
      close(pipe_chain[1]);

      // execute pre post '|' process
      if (execvp(args[0], pre_args) == -1) {
        // execvp() only returns when error overwriting process image
        perror("ERROR [pipe_error; execvp()_1]");
        exit(1);
      }
  } else {                                        // parent process
      pid = fork();                               // fork post process

      if (pid == -1) {                            // no child process created
          perror("ERROR [pipe_error; fork()_2]");
          return;
      } else if (pid == 0) {                      // secondary child process  
          // duplicates read end of pipe to descriptor 0 (originally stdin)
          if (dup2(pipe_chain[0], 0) == -1) {
            perror("ERROR [pipe_error; dup2()_2]");
            exit(1);
          }
          
          // close secondary child process pipe
          close(pipe_chain[0]);
          close(pipe_chain[1]);

          // execute post '|' process
          if (execvp(args[i+1], post_args) == -1) {
            // execvp() only returns when error overwriting process image
            perror("ERROR [pipe_error; execvp()_2]");
            exit(1);
          }
      } else {                                    // rest of parent process
        
        // close parent process pipe
        close(pipe_chain[0]);
        close(pipe_chain[1]);
        
        // wait for both processes to change states
        wait(&wstatus);
        wait(&wstatus);
        return;
      }
  }
}

/* Working Directory ( cd ) */  
void working_dir(char **args, char *rwd) {
  if (args[1] == NULL) {                        // return to root directory
      chdir(rwd);
  } else {                                      // change to provided directory
      if (chdir(args[1]) == -1) {
        // chdir() only returns when unsuccessful directory change
        perror("ERROR [working_dir; chdir()]");
      }
  }
  return;
}

void interpret_args(char **args, char *rwd) {
  if (args[0] == NULL) {                        // handle empty command
      printf (NO_ARGS);
  } else if (!strcmp(args[0], "exit")) {        // handle exit command
      printf (EXIT_MSG);
      exit(0); // exit shell loop
  } else if (!strcmp(args[0], "cd")) {          // handle working directory
      working_dir(args, rwd);
  } else {
      /* search for special characters following a command */
      for (int i = 0; args[i] != NULL; i++) {
        if (!strcmp(args[i], "<")) {            // handle input redirection      
            input_redirect(args, i);            
            i++; // skip file argument
        } else if (!strcmp(args[i], ">")) {     // handle output redirection
            output_redirect(args, i);
            i++; // skip file argument
        } else if (!strcmp(args[i], ">>")) {    // handle append redirection
            append_redirect(args, i);
            i++; // skip file argument
        } else if (!strcmp(args[i], "|")) {     // handle pipes
            pipes(args, i);
            // skip until end of pipe call
            while (args[i+1] != NULL && strcmp(args[i+1], ";")) { i++; }
        } else if (!strcmp(args[i], ">&")) {    // handle output error redirection
            output_error(args, i);
            i++; // skip file argument
        } else if (!strcmp(args[i], ">>&")) {   // handle append error redirection
            append_error(args, i);
            i++; // skip file argument
        } else if (!strcmp(args[i], "|&")) {    // handle pipe error redirection
            pipe_error(args, i);
            // skip until end of pipe call
            while (args[i+1] != NULL && strcmp(args[i+1], ";")) { i++; }
        } else if (args[i+1] == NULL) {         // execute command if end of args
            exec_command(args);
        }
      }
  }
}

int main()
{ 
  char **   args;           // user provided arguments
  char      rwd[PATH_MAX];  // root working directory
  char      cwd[PATH_MAX];  // current working directory

  // get root working directory
  getcwd(rwd, PATH_MAX);
  
  while (1) {               // shell loop
    getcwd(cwd, PATH_MAX);  // get current working directory
    
    // print shell prompt and get args
    printf("[myshell:%s] # ", cwd);
    args = get_args();
    
    // print_args(args);    // prints a column of args  
    // print_args1(args);   // prints a list of args

    // interpret and execute commands
    interpret_args (args, rwd);
	}
}
