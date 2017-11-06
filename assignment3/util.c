//
// Created by Morgan Wu on 11/10/15.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void output_msg1(const char *s) {
    write(STDOUT_FILENO, s, strlen(s));
}

void err_msg1(const char *s) {
    write(STDERR_FILENO, s, strlen(s));
}

int err_ret1(const char *s) {
    err_msg1(s);
    return 1;
}

void err_exit1(const char *s) {
    err_msg1(s);
    exit(1);
}
