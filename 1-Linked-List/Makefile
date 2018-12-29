.PHONY: all debug clean

## Tests
# REQUIRED_INTERFACE - Tests the required interface.
# EXTRA_FUNCTIONALITY - Tests extra functionality.
# ITERATOR_INTERFACE - Tests the iterator interface.
# EXTRA_ITERATOR_FUNCTIONALITY - Tests the extra iterator functionality.
TESTS := REQUIRED_INTERFACE EXTRA_FUNCTIONALITY ITERATOR_INTERFACE EXTRA_ITERATOR_FUNCTIONALITY

all:
	gcc -Wall -pedantic -O3 -std=c99 -Wno-unused-function $(addprefix -D TEST_,$(TESTS)) main.c linked_list.c -lm -o main.out

debug:
	gcc -Wall -pedantic -O3 -std=c99 -Wno-unused-function -D DEBUG_OUTPUT $(addprefix -D TEST_,$(TESTS)) main.c linked_list.c -lm -o main.out

clean:
	rm -f *.o *.out
