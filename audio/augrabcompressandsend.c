#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>

int main() {
  int fd[2];
  pid_t childpid;
 
  pipe(fd);

  if((childpid = fork()) == -1) {
      perror("fork");
      exit(1);
  }
  
  if(childpid == 0) {
    /* Child process*/
    close(fd[1]);
    /* close standard input of child */
    close(0);
    /* Duplicate the input side of pipe to stdin*/
    dup(fd[0]);
    /* Run the Encoder module here*/
    execlp("encoder", "encoder", NULL);
  } else {
    /* Parent process*/    
    close(fd[0]);
    /* close the standard output of parent*/
    close(1);
    /* Duplicate the output side of pipe to stdout*/
    dup(fd[1]);
    /* Run the Audio capture module here */
    execl("audiocapture", "audiocapture", NULL);
  }
  
  
  return 0;
}
