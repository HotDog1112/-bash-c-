#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#define _GNU_SOURCE

int lines_count = 0;
char last_ch = '\0';
int blanklines = 0;
int bytes = 0;
double line = 1;
char *buffer;

struct cat_flags {
    int b_flag;
    int n_flag;
    int e_flag;
    int v_flag;
    int s_flag;
    int t_flag;
};

int change_flag(int argc, char *argv[], struct cat_flags *p, int count);
void flag_print(char buffer, struct cat_flags *p);
void init(struct cat_flags *p);

#endif  // SRC_CAT_S21_CAT_H_
