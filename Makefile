CC = gcc

all: da_proc tests

da_proc: da_proc.c da_proc.h
	$(CC) -Wall -o da_proc da_proc.c

clean:
	rm da_proc

tests:
	$(CC) -Wall -o tests tests.c