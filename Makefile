all: jc

token.o: token.c token.h
	clang -c token.c

jc: jc.c jc.h token.o
	clang -o jc jc.c token.o -lm

clean:
	rm -f *.o
	rm -f trace