//
// Created by mengdongqi on 15-11-03.
//


#include <stdio.h>
#include "demo7.h"

int main() {
    char *s = "I love you , ";
    char *t = "Morgan";
    char s1[100] = "I love you , ";
    char t1[] = "Morgan";
   // s = strcat1(s, t);
    //while (s++)
       // printf("%c", *s++);
    strcat2(s1,t1);
    int i=0;
    while(s1[i]) {
        printf("%c", s1[i]);
        i++;
    }
    return 0;
}

char *strcat1(char *s, char *t) {
    char *s1 = s;
    while (*s++);
    //remalloc()
    while (*s++ = *t++);
    return s1;
}

void strcat2(char *s,char *t){
    int i,j;
    i=j=0;
    while(s[i++]);
    i--;
    while(t[j]){
        s[i]=t[j];
        i++;
        j++;
    }
    s[i]='\0';
}