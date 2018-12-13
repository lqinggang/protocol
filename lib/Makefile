ALL=libwrapsock.so libcrc.so libescape.so libprotocol.so
CFLAGS=-g -L. -L../lib/ -I. -I../include/ -fPIC -shared 

LIB:$(ALL)
lib%.so:%.c ../include/%.h
	$(CC) $< $(CFLAGS) -o $@

.PHONY:clean
clean:
	rm -rf $(ALL) 
