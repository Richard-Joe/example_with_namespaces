/*
 * Demonstrate the operation of UTS namespaces.
 */
#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    struct utsname uts;

    if (sethostname(arg, strlen(arg)) == -1)
        err_exit("sethostname failed");

    if (uname(&uts) == -1)
        err_exit("uname failed");
    printf("uts.nodename in child: %s\n", uts.nodename);

    execv(bash_args[0], bash_args); 

    return 0;
}

int main(int argc, char *argv[])
{
    pid_t child_pid;
    struct utsname uts;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <child-hostname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (uname(&uts) == -1)
        err_exit("uname failed");
    printf("uts.nodename in parent: %s\n", uts.nodename);

    child_pid = clone(child_func, child_stack + STACK_SIZE,
        CLONE_NEWUTS | SIGCHLD, argv[1]);
    if (child_pid == -1)
        err_exit("clone failed");
    printf("PID of child created by clone() is %ld\n", (long)child_pid);

    if (waitpid(child_pid, NULL, 0) == -1)
        err_exit("waitpid failed");
    printf("child has terminated\n");

    return 0;
}