/*
 * A simple demonstration of PID namespaces.
 */
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>

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
    printf("child_func(): PPID = %ld\n", (long)getppid());

    char *mount_point = arg;

    if (mount_point != NULL) {
        mkdir(mount_point, 0555);
        if (mount("proc", mount_point, "proc", 0, NULL) == -1)
            err_exit("mount failed");
        printf("Mounting procfs at %s\n", mount_point);
    }

    // execlp("sleep", "sleep", "5", (char *)NULL);
    execv(bash_args[0], bash_args); 

    return 0;
}

int main(int argc, char *argv[])
{
    pid_t child_pid;

    child_pid = clone(child_func, child_stack + STACK_SIZE,
        CLONE_NEWPID | SIGCHLD, argv[1]);
    if (child_pid == -1)
        err_exit("clone failed");
    printf("PID of child created by clone() is %ld\n", (long)child_pid);

    if (waitpid(child_pid, NULL, 0) == -1)
        err_exit("waitpid failed");
    printf("child has terminated\n");

    return 0;
}