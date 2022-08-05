all: s21_cat test
rebuild: clean s21_cat s21_grep test
s21_cat:
	gcc -Wall -Wextra -Werror -std=c11 ./s21_cat.c -o ./s21_cat
clean: 
	rm -rf *.o ./s21_cat log.txt
test: s21_cat
	sh t_cat.sh
check_all:
	python3 ../../materials/linters/cpplint.py --extensions=c *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem s21_cat.c *.h
