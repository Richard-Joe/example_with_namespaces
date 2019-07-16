#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define err_exit(msg) \
    do { \
        perror(msg); \
        exit(EXIT_FAILURE); \
    } while(0)

#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];

static int child_func(void *arg)
{
    sleep(3);
    puts("======== child ipc queue ========");
    system("ipcs -q");
    puts("=================================");

    return 0;
}

int main(int argc, char *argv[])
{
    pid_t child_pid = clone(child_func, child_stack + STACK_SIZE,
        CLONE_NEWIPC | SIGCHLD, NULL);
    if (child_pid == -1)
        err_exit("clone failed");\

    system("ipcmk -Q");
    puts("======== parent ipc queue ========");
    system("ipcs -q");
    puts("==================================");

    if (waitpid(child_pid, NULL, 0) == -1)
        err_exit("waitpid failed");
    printf("child has terminated\n");
    return 0;
}