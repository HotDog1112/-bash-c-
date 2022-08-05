all: s21_grep test
rebuild: clean s21_grep test
s21_grep:
	gcc -Wall -Wextra -std=c11 ./s21_grep.c -o ./s21_grep
clean: 
	rm -rf *.o ./s21_grep log.txt
test: s21_grep
	sh test.sh
check_all:
	python3 ../../materials/linters/cpplint.py --extensions=c *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem s21_grep.c *.h
leak: s21_grep
	leaks -atExit -- ./s21_grep -i -v -c -l -n -o -h -s -e grep -e do Makefile s21_grep.h
