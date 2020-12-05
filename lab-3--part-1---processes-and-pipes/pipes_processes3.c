#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int main(int argc, char **argv){
 
  int pipefd1[2];
  int pipefd2[2];
  int pid;

  char *sortArgs[] = {"sort", NULL,NULL};
  char *catArgs[] = {"cat", "scores", NULL};
  char *grepArgs[] = {"grep", "Lakers", NULL};
  
  //making my pipes
  pipe(pipefd1);
  pipe(pipefd2);
  pid = fork();
  
  if (pid==0){ 
    // replace standard input with input part of pipe
    dup2(pipefd1[1],1);
    
    // close unused parts of pipes
    close(pipefd1[0]);
    close(pipefd2[1]);
    close(pipefd2[0]);
    close(pipefd1[1]);
    
    // execute grep
    execvp(*catArgs, catArgs);
  }

  else{
    if(fork()==0){
      // replace standard output with the input to sort
      dup2(pipefd1[0],0);
      dup2(pipefd2[1],1);
      
      // close unused parts of pipes
      close(pipefd2[0]);
      close(pipefd1[1]);
      close(pipefd2[1]);
      close(pipefd1[0]);

      // execute grep
      execvp(*grepArgs,grepArgs);
    }
    
    else{ 
      if(fork() ==0){
        //Execute sort
        dup2(pipefd2[0],0);
        
        // close unused parts of pipes
        close(pipefd2[1]);
        close(pipefd1[1]);
        close(pipefd1[0]);
        close(pipefd2[0]);
        
        // execute grep
        execvp(*sortArgs,sortArgs);
      }
    }
  }
  
  close(pipefd2[1]);
  close(pipefd1[1]);
  close(pipefd1[0]);
  close(pipefd2[0]);
  
  wait(NULL);
  wait(NULL);
  wait(NULL);

}
