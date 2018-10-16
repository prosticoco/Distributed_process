# Plagiarized from http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# If you don't want to understand anything, read http://www.gnu.org/software/make/manual/make.html

LDLIBS = -pthread
IDIR = ../include
# Oh dear
ODIR = obj
CC = clang
CFLAGS = -Wall -I$(IDIR)

_DEPS = ack.h addrbook.h error.h mqueue.h receiver.h sender.h structure.h utils.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))
_OBJ = ack.o addrbook.o da_proc.o mqueue.o receiver.o sender.o utils.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

# Recompile C files automatically if header files change
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

da_proc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o da_proc

tests:
	$(CC) $(CFLAGS) -o tests tests.c