server:server.o 
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
server.o:server.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)
.PHONY:clean
clean:
	rm -rf server.o server 
