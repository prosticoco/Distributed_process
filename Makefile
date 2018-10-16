# Plagiarized from http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# If you don't want to understand anything, read http://www.gnu.org/software/make/manual/make.html
CC = gcc
CFLAGS = -Wall
LDLIBS = -pthread
DEPS = ack.h addrbook.h error.h mqueue.h receiver.h sender.h structure.h utils.h
OBJ = ack.o addrbook.o da_proc.o mqueue.o receiver.o sender.o utils.o

# Recompile C files automatically if header files change
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

da_proc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(OBJ) da_proc

tests:
	$(CC) $(CFLAGS) -o tests tests.c