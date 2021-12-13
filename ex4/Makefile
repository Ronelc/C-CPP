FLAGS = -Wall -Wextra -Wvla -Werror -g -lm -std=c99
OBJECTS = vector.o hashmap.o pair.o

.PHONY: all clean

all: libhashmap.a libhashmap_tests.a

libhashmap.a: hashmap.o vector.o pair.o
	ar rcs libhashmap.a hashmap.o vector.o pair.o

libhashmap_tests.a: test_suite.o
	ar rcs libhashmap_tests.a test_suite.o

vector.o: vector.c vector.h
	gcc $(FLAGS) -c vector.c

hashmap.o: hashmap.c hashmap.h pair.h vector.h
	gcc $(FLAGS) -c hashmap.c

pair.o: pair.c pair.h
	gcc $(FLAGS) -c pair.c

test_suite.o: test_suite.c test_suite.h hashmap.h hash_funcs.h
	gcc $(FLAGS) -c test_suite.c

clean:
	rm -f *.o *.a