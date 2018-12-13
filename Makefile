# If you don't want to understand anything, read http://www.gnu.org/software/make/manual/make.html
LDLIBS = -pthread
IDIR = include
SRCDIR = src
TESTDIR = test
# Oh dear
SRCODIR = $(SRCDIR)/obj
TESTODIR = $(TESTDIR)/obj
CC = gcc
CFLAGS = -Wall -I$(IDIR)

_DEPS = addrbook.h data.h dependencies.h error.h causal.h log.h layers.h layerzero.h mqueue.h parser.h pending.h plink.h receiver.h sender.h urb.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))
_SRCOBJ = addrbook.o beb.o da_proc.o dependencies.o causal.o flink.o layerzero.o log.o mqueue.o parser.o pending.o plink.o receiver.o sender.o urb.o
SRCOBJ = $(patsubst %, $(SRCODIR)/%, $(_SRCOBJ))
_TESTOBJ = test.o
TESTOBJ = $(patsubst %, $(TESTODIR)/%, $(_TESTOBJ))

.PHONY: clean debug

# Recompile C files automatically if header files change
$(TESTODIR)/%.o: $(TESTDIR)/%.c $(DEPS)
	@mkdir -p $(TESTODIR)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUG)

$(SRCODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	@mkdir -p $(SRCODIR)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUG)

# Link source object files together
da_proc: $(SRCOBJ)
	@echo "Linking da_proc"
	$(CC) -o $@ $^ $(CFLAGS) $(DEBUG) $(LDLIBS)

tests: $(TESTOBJ) $(SRCOBJ)
	@echo "Linking tests"
	$(CC) -o $@ $(filter-out $(SRCODIR)/da_proc.o, $^) $(CFLAGS) $(DEBUG) $(LDLIBS)

# Debug target
debug:
	@echo "Building with debugging mode."
	@$(MAKE) $(MAKEFILE) DEBUG="-g"

clean:
	rm -rf $(SRCODIR)
	rm -rf $(TESTODIR)
	rm -f tests da_proc
	rm -f *.out
