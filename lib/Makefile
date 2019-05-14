LDFLAGS= -fPIC -shared

SRC=$(wildcard *.c)
OBJS=$(patsubst %.c, lib%.so, $(SRC))

LIB:$(OBJS)

lib%.so:%.c $(ROOT)/include/%.h
	$(CC) $< $(CFLAGS) -o $@ $(LDFLAGS)

.PHONY:clean
clean:
	rm -rf $(OBJS)
