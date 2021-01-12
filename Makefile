CC = gcc -Wall -g

all: frequency
frequency: frequency.o
	$(CC) -o frequency frequency.o
frequency.o: frequency.c
	$(CC) -c frequency.c
.PHONY: all clean
clean:
	rm -f *.o frequency