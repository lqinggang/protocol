LDFLAGS+= -lpthread
client:client.o 
	$(CC) $< $(CFLAGS) -o $@ $(LDFLAGS)
client.o:client.c
	$(CC)  -c $<  $(CFLAGS) -o $@ $(LDFLAGS)
.PHONY:clean
clean:
	rm -rf client.o client 
