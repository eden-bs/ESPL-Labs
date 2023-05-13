
all: 
	make myshell
	make mypipe

myshell: myshell.o
	gcc -g -Wall -c LineParser.c -o LineParser.o
	gcc -g -Wall -c myshell.c -o myshell.o
	gcc -g -Wall  LineParser.o myshell.o -o myshell
	gcc Looper.c -o looper



myshell.o: myshell.c
	$(CC) $(CFLAGS) -c myshell.c -o myshell.o

mypipe: mypipe.c
	gcc -g -Wall -c mypipe.c -o mypipe.o
	gcc -g -Wall  mypipe.o -o mypipe
# mypipe.o: mypipe.c
# 	$(CC) $(CFLAGS) -c mypipe.c -o mypipe.o

LineParser.o: LineParser.c
	$(CC) $(CFLAGS) -c LineParser.c -o LineParser.o


clean:
	rm -f *.o myshell
	rm -f *.o mypipe

