# If you don't want to understand anything, read http://www.gnu.org/software/make/manual/make.html
LDLIBS = -pthread
IDIR = include
SDIR = src
# Oh dear
ODIR = src/obj
CC = gcc
CFLAGS = -Wall -I$(IDIR)

_DEPS = addrbook.h data.h dependencies.h error.h causal.h log.h layers.h layerzero.h mqueue.h parser.h plink.h receiver.h sender.h urb.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))
_OBJ = addrbook.o beb.o da_proc.o dependencies.o causal.o flink.o layerzero.o log.o mqueue.o parser.o plink.o receiver.o sender.o urb.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

# Recompile C files automatically if header files change
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUG)

da_proc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(DEBUG) $(LDLIBS)

# Debug target
debug:
	@echo "Building with debugging mode."
	@$(MAKE) $(MAKEFILE) DEBUG="-g"

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o da_proc
	rm -f tests
	rm -f *.out

tests: test/test.c
	$(CC) $(CFLAGS) -o tests test/test.c -pthread
