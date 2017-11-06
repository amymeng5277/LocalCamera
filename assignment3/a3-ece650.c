//
// Created by mengdongqi on 15-11-08.
//

#include "a3-ece650.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    int n;
    int fd_rgen[2];// for rgen -> main
    int fd_rgen_err[2]; // for rgen_err -> main;
    int fd1_1[2]; // for main -> a1
    int fd1_2[2]; // for a1 -> main
    int fd1_err[2]; // for a1_err -> main

    int fd3[2]; // for main - as2
    char rgen_output[MAXOUTPUT];
    char std_err[MAXOUTPUT];

    pid_t pid_rgen, pid_a1, pid_a2;

    pipe(fd_rgen);
    pipe(fd_rgen_err);
    pipe(fd1_1);
    pipe(fd1_2);
    pipe(fd1_err);
    while(1);
    while ((pid_rgen = fork()) == -1);

    if (pid_rgen == 0) { // in rgen child process
        close(fd_rgen[0]);
        close(fd_rgen_err[0]);
        dup2(fd_rgen_err[1], STDERR_FILENO);
        if (dup2(fd_rgen[1], STDOUT_FILENO) != STDOUT_FILENO)
            err_exit1("Error: dup2 error for rgen to stdout\n");
        if (execv("./rgen", argv) == -1)
            err_exit1("Error: rgen process start error\n");


    } else { // main process
        int status;
        waitpid(pid_rgen, &status, 0);
        close(fd_rgen[1]);
        close(fd_rgen_err[1]);
        printf("%s", "111");
        if ((n = read(fd_rgen_err[0], std_err, MAXOUTPUT)) > 0) {
            std_err[n] = '\0';
            printf("%s", std_err);
        }
        printf("%s", "222");
        if ((n = read(fd_rgen[0], rgen_output, MAXOUTPUT)) > 0) {
            rgen_output[n] = '\0';
            printf("%s", rgen_output);

        }
        printf("%s", "333");


    }


    pid_a1 = fork();
    if (pid_a1 < 0) {
        printf("Error: process a1 is faild");
    } else if (pid_a1 == 0) {
//execl("a1-ece650.py",);
    } else {
        pid_a2 = fork();
        if (pid_a2 < 0) {
            printf("Error: process rgen is faild");
        } else if (pid_a2 == 0) {
// execl("a2-ece650",);
        } else {
            char usrIput[MAXLINE]; // 只是在局部用到的变量坚决不要扩大其作用域，原来这个是在main下面声明的，而且只有char * 是无法存储数据的，即用户的输入无法被存储到usrInput
            char command; // 不是声明指针，而是声明变量本身
            int start, end; // 不是声明指针，而是声明变量本身
            while (
                    fgets(usrIput,
                          MAXLINE, stdin) != NULL) { // 获取一行输入到 usrIput，学习使用这个fgets函数，很有用!
                getInformation(usrIput,
                               &start, &end, &command); // 传入地址
                printf("c:%c s:%d e:%d\n", command, start, end);
            }
        }
    }

}

void getInformation(char array[], int *a, int *b, char *command) {
    int i = 0, j = 0;
//    char *numA, *numB; // 只声明指针是没有数据空间的，无法放入数据，也就是说 numA[j]='1' 会失败
    char numA[32] = "", numB[32] = "";
    if (array[0] == 's')
        *command = 's'; // 其实这一步必须为s，也只能是s，所以理论上说，只需要提取a和b，anyway 先不管这些细节
    i = 1; // 从1开始 因为array[0]已经是s了
    while (array[i] == ' ') i++;// 向后移动到空格后面一位
    while (array[i] >= '0' && array[i] <= '9') {
        numA[j++] = array[i++];
    }
    numA[j] = '\0';
    while (array[i] == ' ') i++;// 向后移动到空格后面一位
    j = 0;
    while (array[i] >= '0' && array[i] <= '9')
        numB[j++] = array[i++];
    numB[j] = '\0';
    *a = atoi(numA);
    *b = atoi(numB);
}
