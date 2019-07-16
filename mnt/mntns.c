#define _GNU_SOURCE
#include <sys/capability.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define err_exit(msg) \
    do { \
        perror(msg); \
        exit(EXIT_FAILURE); \
    } while(0)

#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];

char* const bash_args[] = {
    "/bin/bash",
    NULL
};

static int child_func(void *arg)
{
    printf("child_func(): PID  = %ld\n", (long)getpid());

    char hostname[128];
    snprintf(hostname, sizeof(hostname), "child[%ld]", (long)getpid());
    sethostname(hostname, strlen(hostname));

    system("mount -t proc proc /proc");
    execv(bash_args[0], bash_args); 
    return 0;
}

int main(int argc, char *argv[])
{
    pid_t child_pid = clone(child_func, child_stack + STACK_SIZE,
        CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);
    if (child_pid == -1)
        err_exit("clone failed");\

    if (waitpid(child_pid, NULL, 0) == -1)
        err_exit("waitpid failed");
    printf("child has terminated\n");
    return 0;
}