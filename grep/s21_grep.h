#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#define MAX_SIZE 1024
#define _GNU_S 1
#define _GNU_SOURCE

char *error_message;
int files_count = 0;
int error_count = 0;
char *l_file_name;
double c_count = 0.0;
double c_v_count = 0.0;
int line = 0;
int size_str = 0;
int ret = 0;
char *pat;
int print_name = 0;
char *template;
char *buffer;
char error[MAX_SIZE] = "";
int flag = 0;
int how_match = 0;

struct grep_flags {
    int i_flag;
    int e_flag;
    int v_flag;
    int c_flag;
    int l_flag;
    int n_flag;
    int h_flag;
    int s_flag;
    int f_flag;
    int o_flag;
};

regex_t pattern;

int change_flag(struct grep_flags *p);
int compare_and_print(const char *name, FILE *filename, struct grep_flags *p);
char more_eflag(char *template,  char *optarg);
int count_lines(char *buffer, struct grep_flags *p);
void count_c_v(struct grep_flags *p, char *buffer);
void flags_c_l(struct grep_flags *p);
void flag_print(struct grep_flags *p, char *buffer);
void print_f_flag(char** argv);
void init(struct grep_flags *p);
static char *substr(const char *str, unsigned start, unsigned end);
void o_flag(struct grep_flags *p, FILE *filename);

#endif  // SRC_GREP_S21_GREP_H_
