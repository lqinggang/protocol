client:client.o 
	$(CC) $< $(CFLAGS) -o $@
client.o:client.c
	$(CC)  -c $<  $(CFLAGS) -o $@
.PHONY:clean
clean:
	rm -rf client.o client 
