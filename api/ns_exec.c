/*
 * Join a namespace and execute a command in the namespace
 */
#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define err_exit(msg) \
    do { \
        perror(msg); \
        exit(EXIT_FAILURE); \
    } while(0)

int main(int argc, char *argv[])
{
    int fd;

    if (argc < 3) {
        fprintf(stderr, "%s /proc/PID/ns/FILE cmd [arg...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        err_exit("open failed");

    if (setns(fd, 0) == -1)
        err_exit("setns failed");

    /* execute a command in namespace */
    execvp(argv[2], &argv[2]);
    return 0;
}