CFLAGS=-g -L. -L../lib/ -I. -I../include/ -fPIC -shared 
LIB:libwrapsock.so libcrc.so libescape.so libprotocol.so
lib%.so:*.c
	$(CC) $< $(CFLAGS) -o $@
libwrapsock.so:wrapsock.c
	$(CC)  $< $(CFLAGS) -o $@
libcrc.so:crc.c
	$(CC)  $< $(CFLAGS) -o $@
libescape.so:escape.c
	$(CC)  $< $(CFLAGS) -o $@
libprotocol.so:protocol.c
	$(CC)  $< $(CFLAGS) -o $@

.PHONY:clean
clean:
	rm -rf *.so
