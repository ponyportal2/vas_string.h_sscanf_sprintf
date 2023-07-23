UNAME:=$(shell uname -s)
ifeq ($(UNAME),Linux)
	LFLAGS=-lcheck -lsubunit -lrt -lm -lpthread
	LEAK=valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --log-file="leak.log"
	COMMANDS=dos2unix test_log.sh test_leak.sh
	SYSFLAG=-D LINUX
endif
ifeq ($(UNAME),Darwin)
	LFLAGS=-lcheck -lm
	LEAK=leaks -quiet -atExit --
	SYSFLAG=-D APPLE
	COMMANDS=
endif
CFLAGS=-Wall -Werror -Wextra
GFLAGS=-fprofile-arcs -ftest-coverage
SOURCES=vas_sscanf.c vas_string.c vas_sprintf.c

all: vas_string.a

style:
	clang-format -n -style=Google *.c *.h

vas_string: vas_string.a

vas_string.a: clean
	gcc  $(SYSFLAG) $(CFLAGS) -c $(SOURCES)
	ar rc vas_string.a *.o
	ranlib vas_string.a
	rm -f *.o

libvas_string.a: clean
	gcc  $(SYSFLAG) $(CFLAGS) -c $(SOURCES)
	ar rc libvas_string.a *.o
	ranlib libvas_string.a
	rm -f *.o
	
gcov_report: clean
	gcc $(GFLAGS) $(CFLAGS) $(SYSFLAG) $(SOURCES) test.c $(LFLAGS) -o test
	-./test > test.log
	bash test_log.sh
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html
	
test: vas_string.a
	gcc $(SYSFLAG) $(CFLAGS) test.c vas_string.a -L. $(LFLAGS) -o test
	-./test > test.log
	bash test_log.sh

leaks: vas_string.a
	gcc $(SYSFLAG) $(CFLAGS) test.c vas_string.a -L. $(LFLAGS) -o test
	$(LEAK) ./test > test.log
	$(COMMANDS)
	bash test_log.sh
	bash test_leak.sh

rebuild: clean gcov_report

clean:
	rm -f test vas_string.a libvas_string.a *.gc* test.info log.xml test.log leak.log
	rm -f -rf report