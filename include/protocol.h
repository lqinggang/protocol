/****************************************
   > File Name: protocal.h
   > Author: lqinggang
   > Email: 1944058861@qq.com
   > Create Time: Sat 08 Dec 2018 02:25:22 PM CST
****************************************/


#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "crc.h"
#include "escape.h"


/*
 *  0		8		16		24		32
 *  |-------|--------|-------|-------|
 *  |  head |      length    | option|
 *  |-------|--------|-------|-------|
 *  |  cmd  |  dcmd  |	     |  tail |
 *  |-------|--------|-------|-------|
 *          |  data: N byte  |
 */
#define HEADER 0xAA
#define TAIL 0x55
#define CMD 0x04
#define REPORT 2
#define HEAERBEAR  1

#define version(num) (0x00 | (num << 4))
#define MAXLENGTH 2048

typedef unsigned char byte;

struct interaction {
	byte header; // == 0xAA
	byte length[2]; //length of (option + cmd + data)
	byte option; 
	byte cmd; 
	char data[MAXLENGTH];
	byte crc;
	byte tail; // == 0x55
	byte dcmd;
};

unsigned char *generadata(int dcmd, const void *msg, unsigned char *data, size_t *length);

ssize_t psend(int sockfd, const char *buf, size_t len, int flags);

int resolve(char* dstptr, const char *rscptr, size_t *len);


#endif

