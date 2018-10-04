CC = gcc
CFLAGS = -Wall -std=gnu11

all: da_proc tests

da_proc: da_proc.c utils.c utils.h structure.h error.h
	$(CC) $(CFLAGS) -o da_proc da_proc.c utils.c

clean:
	rm da_proc tests

tests:
	$(CC) $(CFLAGS) -o tests tests.c