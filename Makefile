MAKE=make
CC=gcc
LIBDIR=./lib/
ROOT=$(shell pwd)
LINK=-L. -L./lib/ -I. -I./include/
CFLAGS=-g -L.. -L. -L$(ROOT)/lib/ -I.. -I. -I$(ROOT)/include/ -lwrapsock  -lcrc -lescape -lprotocol
export MAKE CC LIBDIR ROOT LINK CFLAGS
ALL:LIB CLIENT SERVER 
LIB:
	cd $(LIBDIR) && $(MAKE) 
CLIENT:
	cd $(ROOT)/client/ && $(MAKE) 
SERVER:
	cd $(ROOT)/server/ && $(MAKE)

.PHONY:clean
clean:
	cd $(LIBDIR) && rm  -rf *.so
	cd ./client/ && rm -rf client client.o
	cd ./server/ && rm -rf server server.o


