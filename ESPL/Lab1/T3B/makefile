all: t3b

t3b: t3b.o
		gcc -m32 -g -Wall -o t3b t3b.o
t3b.o: t3b.c
		gcc -m32 -g -Wall -c -o t3b.o t3b.c
clean:
		rm -f *.o t3b
