CC := gcc
CFLAGS := -Wall -g -std=c99 $(DEFS)

ex := $(patsubst %.c,%,$(wildcard ex*.c))
obj := $(patsubst %.c,%.o,$(wildcard *.c))
dep := $(obj:.o=.d)

all: $(ex) -o $(ex).o

clean:
	rm -f $(ex)
	rm -f $(obj)
	rm -fR *.dSYM
	rm -fR $(dep)

# Generate dependencies automatically
include $(obj:.o=.d)
%.d: %.c
	$(CC) -MM $(CPPFLAGS) $< | sed 's|$*\.o[ :]*|$*.o $*.d: |g' > $@
