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

char* const bash_args[] = {
    "/bin/bash",
    NULL
};

static int child_func(void *arg)
{
    sleep(3);
    system("ifconfig veth1 10.0.0.4");
    puts("======== child network interfaces ========");
    system("ifconfig -a");
    puts("==========================================");
    sleep(1);
    system("ping -c 3 10.0.0.3");

    execv(bash_args[0], bash_args); 

    return 0;
}

int main(int argc, char *argv[])
{
    pid_t child_pid = clone(child_func, child_stack + STACK_SIZE,
        CLONE_NEWNET | SIGCHLD, NULL);
    if (child_pid == -1)
        err_exit("clone failed");\

    char buf[256];
    sprintf(buf, "ip link add name veth0 type veth peer name veth1 netns %ld", (long)child_pid);
    system(buf);
    system("ifconfig veth0 10.0.0.3");

    puts("======== parent network interfaces ========");
    system("ifconfig -a");
    puts("==========================================");

    if (waitpid(child_pid, NULL, 0) == -1)
        err_exit("waitpid failed");
    printf("child has terminated\n");
    return 0;
}