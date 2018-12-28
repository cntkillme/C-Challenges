.PHONY: all debug clean

## Options
# TEST_REQUIRED_INTERFACE - Tests the required interface.
# TEST_EXTRA_FUNCTIONALITY - Tests extra functionality.
# TEST_ITERATOR_INTERFACE - Tests the iterator interface.
# TEST_EXTRA_ITERATOR_FUNCTIONALITY - Tests the extra iterator functionality.
# DEBUG_OUTPUT - Display extra output.
OPTIONS := TEST_REQUIRED_INTERFACE TEST_EXTRA_FUNCTIONALITY

all:
	gcc -Wall -pedantic -O3 -std=c99 $(addprefix -D, $(OPTIONS)) main.c linked_list.c -o main.out -lm

debug:
	gcc -Wall -pedantic -O3 -std=c99 -D DEBUG_OUTPUT $(addprefix -D, $(OPTIONS)) main.c linked_list.c -o main.out -lm

clean:
	rm -f main.out
