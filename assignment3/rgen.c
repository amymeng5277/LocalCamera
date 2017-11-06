//
// Created by mengdongqi on 15-11-06.
//

#include "rgen.h"
#include "util.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/file.h>
#include <stdlib.h>


/* If we are waiting reading from a pipe and
   the interlocutor dies abruptly (say because
   of ^C or kill -9), then we receive a SIGPIPE
   signal. Here we handle that.
 */
void sig_pipe(int n) {
    perror("Broken pipe signal");
}


/* Handler for SIGCHLD signal */
void sig_chld(int n) {
    int status;

    fprintf(stderr, "Child terminated\n");
    wait(&status); /* So no zombies */
}


void daemon_init(const char *const path, uint mask) {
    pid_t pid;
    char buff[256];
    static FILE *log; /* for the log */
    int fd;
    int k;

    /* put server in background (with init as parent) */
    if ((pid = fork()) < 0) {
        perror("daemon_init: cannot fork");
        exit(0);
    } else if (pid > 0) /* The parent */
        exit(0);

    /* the child */

    /* Close all file descriptors that are open */
    for (k = getdtablesize() - 1; k > 0; k--)
        close(k);

    /* Redirecting stdin and stdout to /dev/null */
    if ((fd = open("/dev/null", O_RDWR)) < 0) {
        perror("Open");
        exit(0);
    }
    dup2(fd, STDIN_FILENO);      /* detach stdin */
    close(fd);
    /* From this point on printf and scanf have no effect */

    /* Redirecting stderr to unixlog_path */
    log = fopen("/tmp/rgen.log", "aw"); /* attach stderr to unixlog_path */
    fd = fileno(log);  /* obtain file descriptor of the log */
    dup2(fd, STDERR_FILENO);
    dup2(fd, STDOUT_FILENO);     /* detach stdout */
    close(fd);
    /* From this point on printing to stderr will go to /tmp/mylog */

    /* Establish handlers for signals */
    if (signal(SIGCHLD, sig_chld) < 0) {
        perror("Signal SIGCHLD");
        exit(1);
    }
    if (signal(SIGPIPE, sig_pipe) < 0) {
        perror("Signal SIGPIPE");
        exit(1);
    }

    /* Change directory to specified directory */
    chdir(path);

    /* Set umask to mask (usually 0) */
    umask(mask);

    /* Detach controlling terminal by becoming sesion leader */
    setsid();

    /* Put self in a new process group */
    pid = getpid();
    setpgrp(); /* GPI: modified for linux */

    /* Make sure only one server is running */
    if ((k = open("/tmp/rgen.log", O_RDWR | O_CREAT, 0666)) < 0)
        exit(1);
    if (lockf(k, F_TLOCK, 0) != 0)
        exit(0);

    /* Save server's pid without closing file (so lock remains)*/
    sprintf(buff, "%6d", pid);
    write(k, buff, strlen(buff));

    return;
}


int main(int argc, char *argv[]) {
//    daemon_init("/", 0);
    char s[MAXLINE];
    int nodeScaleUpper = DF_NODESCALE_UPPER,
            nodeScaleLower = DF_NODESCALE_LOWER,
            streetSegCountUpper = DF_ST_SEGMENT_UPPER,
            streetSegCountLower = DF_ST_SEGMENT_lOWER,
            streetCountUpper = DF_ST_UPPER,
            streetCountLower = DF_ST_LOWER,
            waitTime = DF_WAITTIME;
    int current_cursor = 1;
    while (current_cursor < argc) {
        char *c = argv[current_cursor++];
        char *n = argv[current_cursor++];
        switch (c[1]) {
            case 'c':
                nodeScaleUpper = atoi(n);
                nodeScaleLower = -atoi(n);
                break;
            case 'l':
                waitTime = atoi(n);
                break;
            case 'n':
                streetSegCountUpper = atoi(n);
                break;
            case 's':
                streetCountUpper = atoi(n);
                break;
            default:
                err_exit1("Error: Command is not correct!");
                break;
        }
    }
//    while (1) {
        int i = 0,
                j = 0,
                street_number = randnum(streetCountUpper, streetCountLower),
                segmentCount = 0;
        pNodeList street_list[street_number];
        while (i < street_number) {
            segmentCount = randnum(streetSegCountUpper, streetSegCountLower);
            street_list[i] = generate_multi_nodes(nodeScaleUpper, nodeScaleLower, nodeScaleUpper, nodeScaleLower, segmentCount);
            sprintf(s, "a \"%d\" ", i);
            output_msg1(s);
            while (j < segmentCount) {
                sprintf(s, "(%d,%d)", street_list[i][j].x, street_list[i][j].y);
                output_msg1(s);
                j++;
            }
            write(STDOUT_FILENO, "\n", strlen("\n"));
            j = 0;
            i++;
        }
        write(STDOUT_FILENO, "g\n", strlen("g\n"));
        freeNodeList(street_list, street_number);
//        sleep(waitTime);
//    }
    exit(0);
}

void freeNodeList(pNodeList street_list[], int size) {
    int i = 0;
    while (i < size) {
        free(street_list[i]);
        i++;
    }
}
