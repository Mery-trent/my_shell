/* my_shell.c : the shell game
* A tiny shell that resognizes a fixed set of verbs. for each 
verb it  forks a child, the child replaces itself with the equivalent 
bash command using execvp(), and the parent wait()s for the child to finish 
(No Zompie processes left behind )

* Verbs                               Bash equivanel
----------------------------------------------------------------------------
help                                (Prints the help menu, no fork)
memory                               free -h
biggest [dir]                        ls -lhS <dir>  (default: .)
procs [username]                     ps -u <user> -o pid, stat, etime, cmd --sort=-etime
online                               who -H
logout                               exits the shell, no fork
****
compile: gcc -Wall -o myshell assn4.c            
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LINE_MAX_LEN 256
#define ARG_MAX_LEN 128
#define MAX_ARGS   16
