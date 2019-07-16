/*
 * Demonstrate the use of the clone() CLONE_NEWUSER flag.
 * 需要使用root权限来执行
 */
#define _GNU_SOURCE
#include <sys/capability.h>
#include <sys/wait.h>
#include <sched.h>
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

static int child_func(void *arg)
{
    cap_t caps;

    printf("eUID = %ld; eGID = %ld;\n", (long)geteuid(), (long)getegid());

    caps = cap_get_proc();
    printf("capabilities: %s\n", cap_to_text(caps, NULL));

    return 0;
}

int main(int argc, char *argv[])
{
    pid_t child_pid;

    child_pid = clone(child_func, child_stack + STACK_SIZE,
        CLONE_NEWUSER | SIGCHLD, NULL);
    if (child_pid == -1)
        err_exit("clone failed");\

    if (waitpid(child_pid, NULL, 0) == -1)
        err_exit("waitpid failed");
    printf("child has terminated\n");

    return 0;
}