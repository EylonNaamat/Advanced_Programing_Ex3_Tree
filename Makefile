all: stree

stree : stree.o
	gcc stree.o -o stree

stree.o : stree.c
	gcc -c stree.c

.PHONY: clean

clean:
	rm *.o stree

