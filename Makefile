CC = gcc
CFLAGS = -Wall

all: da_proc tests

da_proc: da_proc.c da_proc.h utils.c utils.h
	$(CC) $(CFLAGS) -o da_proc da_proc.c utils.c

clean:
	rm da_proc tests

tests:
	$(CC) $(CFLAGS) -o tests tests.c