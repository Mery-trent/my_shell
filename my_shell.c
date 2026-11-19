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

/***************
 * Helper Function: fork a child, exec the command, wait for it. 
 * 
 * argv must be a null-terminated array; argv[0] is the program name
************* */
static void run_comand ( char *const argv[])
{
    pid_t pid = fork();

    // case of failed fork
    if(pid < 0){
        perror("fork");
        return;
    }
    // the child process
    if(pid == 0){
        execvp(argv[0], argv);
        // case of exec failed
        perror(argv[0]);
        exit(127);  // comand not found

    }
    // parent wait for this child
    int status;
    while (waitpid(pid, &status,0)<0){
        // retry only if interrupted
        if (errno ! EINIR){    
            perror("wait");
            return;
        }
    }
}
/***********************
 * a function to get the username fron the environment variable
 **********************/
static const char *current_user(void)
{
    const char *u = getenv("USER");
    return (u!= NULL && *u != '\0')? u: "root";

}
/*********************
 * Verb Handlers
 **********************/
// function that handle the verp "help", it displays a menu of all
// me verbs with thier functionality
static void do_help(void)
{
    printf("Valid verbs are: \n"
            "\thelp:                Display the help menu\n"
            "\tmemory:              shows total/used/free RAM and swap\n"
            "\tbiggest [directory]  lists files in a directory, largest first\n"
            "\tprocs [username]     shows a user's process, longest-running first\n"
            "\tonline               shows who is currently logged in\n"
            "\tlogout               exits this shell and returns to bash\n"
            "\tNOTE: elements in square brackets are optional. Should you\n"
            "\tomit them, the current directory / current username is used.\n" );
            
}