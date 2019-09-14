.PHONY: all debug clean

all:
	gcc -Wall -Werror -pedantic -O3 -std=c99 main.c table.c -o main.out

debug:
	gcc -Wall -Werror -pedantic -O3 -std=c99 -D DEBUG_OUTPUT main.c table.c -o main.out

clean:
	rm *.o *.out
