/*
 * Create a series of child processes in nested PID namespaces.
 */
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <limits.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>

#define err_exit(msg) \
    do { \
        perror(msg); \
        exit(EXIT_FAILURE); \
    } while(0)

#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];

static int child_func(void *arg)
{
    static int first_call = 1;
    long level = (long)arg;

    if (!first_call) {
        char mount_point[PATH_MAX];

        snprintf(mount_point, PATH_MAX, "/proc%c", (char)('0' + level));

        mkdir(mount_point, 0555);
        if (mount("proc", mount_point, "proc", 0, NULL) == -1)
            err_exit("mount failed");
        printf("Mounting procfs at %s\n", mount_point);
    }

    first_call = 0;

    if (level > 0) {
        level--;
        pid_t child_pid;

        child_pid = clone(child_func, child_stack + STACK_SIZE,
            CLONE_NEWPID | SIGCHLD, (void *)level);
        if (child_pid == -1)
            err_exit("clone failed");
        printf("PID of child created by clone() is %ld\n", (long)child_pid);

        if (waitpid(child_pid, NULL, 0) == -1)
            err_exit("waitpid failed");
        printf("child has terminated\n");
    } else {
        printf("Final child sleeping\n");
        execlp("sleep", "sleep", "5", (char *) NULL);
        return 0;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    long levels;

    levels = (argc > 1) ? atoi(argv[1]) : 5;
    child_func((void *) levels);

    exit(EXIT_SUCCESS);
}