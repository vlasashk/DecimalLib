CC = gcc
C_LIBS = -lm
TEST_LIBS = -lpthread -lcheck -lm
CFLAGS = -std=c11 -Wall -Wextra -Werror 
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))

all: build

build: test

test: s21_decimal.a
	$(CC) $(CFLAGS) Tests/*.c s21_decimal.a $(C_LIBS) -o test
	./test


s21_decimal.a: $(OBJECTS)
	ar rcs s21_decimal.a $(OBJECTS)
	ranlib s21_decimal.a

gcov_flag:
	$(eval CFLAGS += --coverage)

debug_flag:
	$(eval CFLAGS += -g)

gcov_report: clean gcov_flag test
	mkdir -p gcov
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory gcov
	rm -rf *.gcno *.gcda *.gcov
	
debug: clean debug_flag build
	rm -rf *.o

rebuild: clean all

valgrind:
	CK_FORK=no valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./test

clean:
	rm -rf *.a *.o Tests/*.o test *.a *.gcno *.gcda *.gcov *.info
