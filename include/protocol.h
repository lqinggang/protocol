#ifndef PROTOCOL_INCLUDE_PROTOCOL_H
#define PROTOCOL_INCLUDE_PROTOCOL_H

#include <stdio.h>

/*
 *  0		8		16		24		32
 *  |-------|--------|-------|-------|
 *  |  head |      length    | option|
 *  |-------|--------|-------|-------|
 *  |  cmd  |  dcmd  |	   data      |
 *  |-------|--------|-------|-------|
 *  |  data: N byte  |  crc  |  tail |
 */

#define HEADER          0xAA
#define TAIL            0xFF
#define CMD             0x01
#define ESCAPEBYTE      0xCC
#define REPORT          2
#define HEARTBEAT       1

#define version(num)    (0x00 | (num << 4))
#define MAXLENGTH       2048

#define     TIMEOUT     64
#define     INTERVAL    16

#define HEARTREQUEST    "Are you still there"
#define HEARTRESPOND    "Yes, I'm"

typedef  char byte;

struct interaction {
	byte header: 8;     // == 0xAA
	byte length[2];     //length of (option + cmd + data)
	byte option: 8;   
	byte cmd: 8;        //== CMD
	char data[MAXLENGTH];
	byte crc: 8;
	byte tail: 8;       // == 0x55
	byte dcmd: 8;       //0x01: HEARTBEAT; 0x02:REPORT
};

extern int resolve(char *dstptr, const char *rscptr, ssize_t *len);
extern ssize_t psend(int dcmd, int sockfd, const void *buf, size_t len, int flags);
extern ssize_t precv(int sockfd, void *buf, size_t len, int flags);

#endif  /* PROTOCOL_INCLUDE_PROTOCOL_H */
