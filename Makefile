CC = gcc
CFLAGS = -Wall

all: da_proc tests

da_proc: da_proc.c utils.c ack.c ack.h utils.h structure.h error.h addrbook.c addrbook.h sender.c sender.h receiver.c receiver.h
	$(CC) $(CFLAGS) -o da_proc da_proc.c utils.c ack.c addrbook.c addrbook.h sender.c sender.h receiver.c receiver.h -pthread

clean:
	rm da_proc tests

tests:
	$(CC) $(CFLAGS) -o tests tests.c