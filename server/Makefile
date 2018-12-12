server:server.o 
	$(CC) $< $(CFLAGS) -o $@ 
server.o:server.c
	$(CC)  -c $<  $(CFLAGS) -o $@
.PHONY:clean
clean:
	rm -rf server.o server 
