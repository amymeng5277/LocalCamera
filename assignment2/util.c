//
// Created by mengdongqi on 15-10-24.
//

#include <stdio.h>
#include <string.h>
#include "util.h"


// get length of the string format of this num
int num_str_length(char *s, int num) {
    sprintf(s, "%d", num);
    int num_length = strlen(s);
    return num_length;
}
