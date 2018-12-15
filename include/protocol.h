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
#include <sys/select.h>
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
#define HEARTBEAT  1

#define version(num) (0x00 | (num << 4))
#define MAXLENGTH 2048

typedef unsigned char byte;

struct interaction {
	byte header; // == 0xAA
	byte length[2]; //length of (option + cmd + data)
	byte option;   
	byte cmd;  //== CMD
	char data[MAXLENGTH];
	byte crc;
	byte tail; // == 0x55
	byte dcmd; //0x01: HEARTBEAT; 0x02:REPORT
};


ssize_t psend(int dcmd, int sockfd, const void *buf, size_t len, int flags);

ssize_t precv(int sockfd, void *buf, size_t len, int flags);



#endif

