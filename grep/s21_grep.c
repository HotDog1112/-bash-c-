#include "./s21_grep.h"

int main(int argc, char *argv[]) {
    int c = 0;
    struct grep_flags p;
    init(&p);
    while ((c = getopt(argc, argv, "ie:nhsvclo")) != -1) {
        switch (c) {
        case 'i':
            p.i_flag = 1;
            break;
        case 'e':
            p.e_flag++;
            if (p.e_flag > 1) {
                more_eflag(template, optarg);
            } else {
                template = optarg;
            }
            break;
        case 'n':
            p.n_flag = 1;
            break;
        case 'v':
            p.v_flag = 1;
            break;
        case 'c':
            p.c_flag = 1;
            break;
        case 'l':
            p.l_flag = 1;
            break;
        case 'h':
            p.h_flag = 1;
            break;
        case 's':
            p.s_flag = 1;
            break;
        case 'o':
            p.o_flag = 1;
            break;
        }
    }
    if (p.v_flag) {
        p.o_flag = 0;
    }
    flag = change_flag(&p);
    if (p.e_flag) {
        pat = template;
    } else {
        pat = argv[optind];
    }
    optind++;
    int i = 0;
    if (p.e_flag) {
        i = optind - 1;
    } else if (p.o_flag && !p.e_flag) {
        i = optind;
        template = argv[optind - 1];
    } else {
        i = optind;
        template = argv[optind];
    }
    FILE *filename = NULL;
    while (i < argc) {
        filename = fopen(argv[i], "rb");
        if (filename != NULL) {
            files_count = argc - optind;
            l_file_name = argv[i];
            if (p.o_flag || (!p.v_flag && p.o_flag)) {
                o_flag(&p, filename);
            } else if (!p.o_flag) {
                compare_and_print(argv[i], filename, &p);
            }
            line = 0;
            c_count = 0;
            c_v_count = 0;
            fclose(filename);
        } else {
            if (!p.s_flag) {
                printf("%s\n", strerror(errno));
                if (error_count > 1) {
                    break;
                }
            }
            error_count++;
        }
        i++;
    }
    free(buffer);
    return error_count != 0;
}

int change_flag(struct grep_flags *p) {
    int f = REG_NOSUB;
    if (p->i_flag) {
        f |= REG_ICASE;
    }
    if (p->e_flag) {
        f |= REG_EXTENDED;
    }
    return f;
}

int compare_and_print(const char *name, FILE *filename, struct grep_flags *p) {
    size_t size = 0;
    regcomp(&pattern, pat, flag);
    while (getline(&buffer, &size, filename) != -1) {
        line++;
        size_str = (int)strlen(buffer);
        ret = regexec(&pattern, buffer, 0, NULL, 0);
        if (ret != 0) {
            if (ret != REG_NOMATCH && !p->s_flag) {
                regerror(ret, &pattern, error, sizeof error);
                fprintf(stderr, "%s: file %s: %s\n", error_message, name, error);
            } else if (p->v_flag && ret == REG_NOMATCH) {
                c_v_count++;
                flag_print(p, buffer);
            }
        } else {
            c_count++;
            if (!p->c_flag && !p->l_flag && !p->v_flag) {
                flag_print(p, buffer);
            }
        }
        print_name = 1;
    }
    flags_c_l(p);
    regfree(&pattern);
    return print_name;
}

void flag_print(struct grep_flags *p, char *buffer) {
    if (!p->c_flag && !p->l_flag) {
        if (((files_count > 1 && !p->n_flag) ||
        (p->e_flag && files_count > 0 && !p->n_flag)) && !p->h_flag && (how_match != line)) {
            printf("%s:%s", l_file_name, buffer);
        } else if (p->n_flag && (how_match != line)) {
            if (((files_count > 1) || (p->e_flag && files_count > 0)) && !p->h_flag) {
                printf("%s:%d:%s", l_file_name, line, buffer);
            } else {
                printf("%d:%s", line, buffer);
            }
        } else {
            printf("%s", buffer);
        }
        if (p->o_flag && !p->c_flag && !p->l_flag && !p->v_flag) {
            printf("\n");
        } else if ((int)buffer[size_str - 1] != 10 && !p->o_flag) {
            printf("\n");
        }
    }
    how_match = line;
}

void flags_c_l(struct grep_flags *p) {
     if (p->c_flag) {
        if (((files_count > 1 && !p->v_flag && !p->l_flag) ||
        (files_count > 0 && p->e_flag && !p->v_flag && !p->l_flag)) &&
        !p->h_flag) {
            printf("%s:%.0f\n", l_file_name, c_count);
            c_count = 0;
        } else if (p->v_flag) {
            if (p->l_flag) {
                c_v_count = 1;
            }
            if ((files_count > 1 || (p->e_flag && files_count > 0)) && !p->h_flag) {
                printf("%s:%.0f\n", l_file_name, c_v_count);
            } else {
                printf("%.0f\n", c_v_count);
            }
        } else if (p->l_flag) {
            if (c_count > 0) {
                c_count = 1;
            }
            if ((files_count > 1 || (files_count > 0 && p->e_flag)) && !p->h_flag) {
                if (c_count > 0) {
                    printf("%s:%.0f\n%s\n", l_file_name, c_count, l_file_name);
                } else {
                    printf("%s:%.0f\n", l_file_name, c_count);
                }
            } else {
                if (c_count > 0 && !p->h_flag) {
                    printf("%.0f\n%s\n", c_count, l_file_name);
                } else {
                    printf("%.0f\n", c_count);
                }
            }
        } else {
           printf("%.0f\n", c_count);
        }
    }
    if ((((p->l_flag && p->v_flag) || (p->l_flag && p->h_flag)) ||
    (p->l_flag && !p->c_flag && !p->v_flag && !p->h_flag)) &&
    (c_count > 0 || c_v_count > 0)) {
        printf("%s\n", l_file_name);
    }
}

char more_eflag(char *template, char *optarg) {
    char c[10] = "";
    c[0] = '|';
    char *temp = NULL;
    temp = malloc(sizeof(char) * MAX_SIZE);
    strcpy(temp, template);
    template[0] = '\0';
    temp = strncat(temp, c, 1);
    temp = strncat(temp, optarg, 100);
    strncat(template, temp, 100);
    free(temp);
    return *template;
}

void init(struct grep_flags *p) {
    p->i_flag = 0;
    p->e_flag = 0;
    p->v_flag = 0;
    p->c_flag = 0;
    p->l_flag = 0;
    p->n_flag = 0;
    p->h_flag = 0;
    p->o_flag = 0;
    p->s_flag = 0;
}

void o_flag(struct grep_flags *p, FILE *filename) {
    size_t size = 0;
    size_t byte_size = 0;
    regex_t reegex;
    const size_t nmatch = 4026;
    size = getline(&buffer, &byte_size, filename);
    while ((int)size != -1) {
        line++;
        regmatch_t pm[4026];
        if (regcomp(&reegex, template, REG_EXTENDED) == 0) {
        int result = regexec(&reegex, buffer, nmatch, pm, 0);
            if (result == 0 && !p->v_flag) {
                int x = 0;
                c_count++;
                char *print_str = substr(buffer, pm[x].rm_so, pm[x].rm_eo);
                if (!p->c_flag && !p->l_flag) {
                    flag_print(p, print_str);
                }
                line--;
                char *match_substr = substr(buffer, pm[0].rm_eo, strlen(buffer));
                strcpy(buffer, match_substr);
                free(print_str);
                free(match_substr);
        } else {
            size = 0;
        }
    }
    if (size == 0) {
        size = getline(&buffer, &byte_size, filename);
    }
    regfree(&reegex);
    }
    flags_c_l(p);
}

static char *substr(const char *str, unsigned start, unsigned end) {
    int a = end - start;
    char *buf_str = (char *)malloc(sizeof(char) * (a + 1));
    if (buf_str == NULL) {
        exit(0);
    }
    buf_str[a] = '\0';
    strncpy(buf_str, str + start, a);
    return buf_str;
}
