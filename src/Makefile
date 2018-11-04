# Plagiarized from http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# If you don't want to understand anything, read http://www.gnu.org/software/make/manual/make.html

LDLIBS = -pthread
IDIR = ../include
# Oh dear
ODIR = obj
CC = clang
CFLAGS = -Wall -I$(IDIR)

_DEPS = addrbook.h beb.h data.h error.h flink.h parser.h plink.h set.h urb.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))
_OBJ = addrbook.o beb.o da_proc.o flink.o parser.o plink.o set.o urb.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

# Recompile C files automatically if header files change
$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p obj
	$(CC) -c -o $@ $< $(CFLAGS)

da_proc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o da_proc

tests:
	$(CC) $(CFLAGS) -o tests tests.c
