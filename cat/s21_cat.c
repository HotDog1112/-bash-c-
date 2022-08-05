#include "./s21_cat.h"

int main(int argc, char *argv[]) {
    struct cat_flags pi;
    init(&pi);
    int count = 0;
    change_flag(argc, argv, &pi, count);
    return 0;
}

int change_flag(int argc, char *argv[], struct cat_flags *p, int count) {
    int index = 0, f = 0;
    for (count = 1; count < argc; count++) {
         if (argv[count][0] == '-') {
            index = 1;
            while (argv[count][index] != 0) {
               switch (argv[count][index]) {
                  case 'b':
                     p->b_flag = 1;
                     break;
                  case 'n':
                     p->n_flag = 1;
                     break;
                  case 'e':
                     p->e_flag = 1;
                     p->v_flag = 1;
                     break;
                  case 's':
                     p->s_flag = 1;
                     break;
                  case 't':
                     p->t_flag = 1;
                     p->v_flag = 1;
                     break;
                  case 'T':
                     p->t_flag = 1;
                     break;
                  case 'E':
                     p->t_flag = 1;
                     break;
                  case 'v':
                     p->v_flag = 1;
                     break;
                  case '-':
                     index = strlen(argv[count]);
                     if (index == 2) {
                        printf("Flag %s doesn't exist", argv[count]);
                        exit(0);
                     } else {
                        if (!strcmp(argv[count], "--number-nonblank")) {
                           p->b_flag = 1;
                           f = 1;
                        } else if (!strcmp(argv[count], "--number")) {
                           p->n_flag = 1;
                           f = 1;
                        } else if (!strcmp(argv[count], "--squeeze-blank")) {
                           p->s_flag = 1;
                           f = 1;
                        }
                        index--;
                        break;
                    }
               }
               if (argv[count][index + 1] && !(argv[count][index + 1] == 'n' ||
               argv[count][index + 1] == 'b' || argv[count][index + 1] == 'e' ||
               argv[count][index + 1] == 's' || argv[count][index + 1] == 't' ||
               argv[count][index + 1] == 'v' || argv[count][index + 1] == 'T' ||
               argv[count][index + 1] == 'E') && f == 0) {
                  printf("Flag %s doesn't exist", argv[count]);
                  exit(0);
               } else if (argv[count][index] && !(argv[count][index] == 'n' ||
               argv[count][index] == 'b' || argv[count][index] == 'e' ||
               argv[count][index] == 's' || argv[count][index] == 't' ||
               argv[count][index] == 'v' || argv[count][index] == 'T' ||
               argv[count][index] == 'E') && f == 0) {
                  printf("Flag %s doesn't exist", argv[count]);
                  exit(0);
               } else { index++; }
            }
            if (argv[count][1] != 0) {
               for (index = count; index < argc - 1; index++) {
                  argv[index] = argv[index + 1];
               }
               count--;
               argc--;
            }
         }
      }
    for (count = 1; count < argc; count++) {
      FILE *filename = fopen(argv[count], "r");
      if (filename != NULL) {
         char ch = getc(filename);
         last_ch = '\n';
         while (!feof(filename)) {
            flag_print(ch, p);
            ch = getc(filename);
         }
         if (feof(filename)) {
            line = 1.0;
         }
         fclose(filename);
      } else {
         printf("%s: %s: %s\n", argv[0], argv[1], strerror(errno));
      }
      }
    return index;
}

void flag_print(char ch, struct cat_flags *p) {
      if ((last_ch == '\n') && (last_ch == ch)) {
         blanklines++;
      } else {
         blanklines = 0;
      }
      if (!p->s_flag || blanklines < 2) {
         int flag_next = 0;
         if ((last_ch == '\n') && !(ch == '\n' && p->b_flag)) {
            if (p->n_flag || p->b_flag) {
               printf("%6.0f\t", line);
            }
            line++;
         }
         if ((p->e_flag) && ch == '\n') {
            printf("$");
         } else if (p->t_flag) {
            if (ch == '\t') {
               printf("^");
               printf("%c", (char)(64 + ch));
               flag_next = 1;
            }
         }
         if (p->v_flag) {
            if (ch >= 0 && ch < 32 && ch != '\n' && ch != '\t') {
               printf("^");
               printf("%c", (char)(64 + ch));
               flag_next = 1;
            } else if (ch == 127) {
               printf("^?");
               flag_next = 1;
            }
         }
         if (!flag_next) {
            printf("%c", ch);
         }
      }
      last_ch = ch;
}

void init(struct cat_flags *p) {
    p->b_flag = 0;
    p->t_flag = 0;
    p->s_flag = 0;
    p->e_flag = 0;
    p->v_flag = 0;
    p->n_flag = 0;
}
