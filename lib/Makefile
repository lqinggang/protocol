CFLAGS=-g -L. -L../lib/ -I. -I../include/ -fPIC -shared 
LIB:libwrapsock.so libcrc.so libescape.so libprotocol.so
lib%.so:*.c
	$(CC) $< $(CFLAGS) -o $@
libwrapsock.so:wrapsock.c ../include/wrapsock.h
	$(CC)  $< $(CFLAGS) -o $@
libcrc.so:crc.c ../include/crc.h
	$(CC)  $< $(CFLAGS) -o $@
libescape.so:escape.c ../include/escape.h
	$(CC)  $< $(CFLAGS) -o $@
libprotocol.so:protocol.c ../include/wrapsock.h
	$(CC)  $< $(CFLAGS) -o $@

.PHONY:clean
clean:
	rm -rf *.so
