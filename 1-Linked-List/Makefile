.PHONY: all clean0

# Tests
# TEST_REQUIRED_INTERFACE - Tests the required interface.
# TEST_EXTRA_FUNCTIONALITY - Tests extra functionality.
# TEST_ITERATOR_INTERFACE - Tests the iterator interface.
# TEST_EXTRA_ITERATOR_FUNCTIONALITY - Tests the extra iterator functionality.
TESTS := TEST_REQUIRED_INTERFACE

all:
	gcc -Wall -pedantic -O3 $(addprefix -D, $(TESTS)) -std=c99 main.c linked_list.c -o main.out -lm

clean:
	rm -f main.out
