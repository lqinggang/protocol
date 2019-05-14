#ifndef PROTOCOL_INCLUDE_PROTOCOL_H
#define PROTOCOL_INCLUDE_PROTOCOL_H

#include <stdio.h>

/*
 *  0		8		16		24		32
 *  |-------|--------|-------|-------|
 *  |  head |      length    | option|
 *  |-------|--------|-------|-------|
 *  |  cmd  |  dcmd  |	     |  tail |
 *  |-------|--------|-------|-------|
 *          |  data: N byte  |
 */

#define HEADER          0xAA
#define TAIL            0x55
#define CMD             0x04
#define ESCAPEBYTE      0xCC
#define REPORT          2
#define HEARTBEAT       1

#define version(num)    (0x00 | (num << 4))
#define MAXLENGTH       2048

typedef  char byte;

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

extern int resolve(char *dstptr, const char *rscptr, ssize_t *len);
extern ssize_t psend(int dcmd, int sockfd, const void *buf, size_t len, int flags);
extern ssize_t precv(int sockfd, void *buf, size_t len, int flags);

#endif  /* PROTOCOL_INCLUDE_PROTOCOL_H */
