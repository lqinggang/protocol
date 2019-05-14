MAKE=make
CC=gcc
LIBDIR=./lib/
ROOT=$(shell pwd)
LDFLAGS= -lwrapsock  -lcrc -lescape -lprotocol
CFLAGS= -g -Wall -W -L. -L.. -L$(ROOT)/lib/ -I. -I.. -I$(ROOT)/include/
export MAKE CC LIBDIR ROOT LDFLAGS CFLAGS

ALL:LIB CLIENT SERVER 
LIB:
	make -C $(LIBDIR)
CLIENT:
	make -C $(ROOT)/client
SERVER:
	make -C $(ROOT)/server

.PHONY:clean
clean:
	make -C $(LIBDIR) clean
	make -C $(ROOT)/client clean
	make -C $(ROOT)/server clean


