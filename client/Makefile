client:client.o 
	$(CC) $< $(CFLAGS) -lpthread -o $@
client.o:client.c
	$(CC)  -c $<  $(CFLAGS) -lpthread -o $@
.PHONY:clean
clean:
	rm -rf client.o client 
