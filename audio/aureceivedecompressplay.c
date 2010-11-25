/*
Program using pipes to integrate decoder and player.
Copyright (C) 2010  SHREERAM RAMAMURTHY SWAMINATHAN

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

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
    /* Run the audio playback module here*/
    execl("audioplayback", "audioplayback", NULL);
  } else {
    /* Parent process*/    
    close(fd[0]);
    /* close the standard output of parent*/
    close(1);
    /* Duplicate the output side of pipe to stdout*/
    dup(fd[1]);
    /* Run the decoder module here */
    execl("decoder", "decoder", NULL);
  }
  
  
  return 0;
}
