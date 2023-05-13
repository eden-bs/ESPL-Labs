
CC = gcc
CFLAGS = -g -Wall -Werror

.PHONY: all clean

all: hexaPrint Bubblesort AntiVirus

hexaPrint: hexaPrint.c
	$(CC) $(CFLAGS) -o hexaPrint hexaPrint.c

Bubblesort: Bubblesort.c
	$(CC) $(CFLAGS) -o Bubblesort Bubblesort.c

AntiVirus: AntiVirus.c
	$(CC) $(CFLAGS) -o AntiVirus AntiVirus.c
clean:
	rm -f hexaPrint Bubblesort AntiVirus