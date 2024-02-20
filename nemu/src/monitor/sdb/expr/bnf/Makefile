CFLAGS=-std=c11 -g -static

bnf: bnf.c

test: bnf
	./bnf "11+ 22  - (8+2) * 2  * 5"

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean