/****************************************
  > File Name: protocal.c
  > Author: lqinggang
  > Email: 1944058861@qq.com
  > Create Time: Sat 08 Dec 2018 03:19:04 PM CST
 ****************************************/

#include "protocol.h"

static unsigned char *heartbeat(struct interaction interac, unsigned char *data, size_t *length);
static unsigned char *report(struct interaction interac, const unsigned char *msg, unsigned char *data, size_t *length);
static unsigned char cal_crc(struct interaction interac);
static unsigned char *getdata(struct interaction interac, unsigned char *data, size_t *length);

/*
 * dcmd: the data command type
 * msg: the message, need to send
 * data: save the generated data
 * length: length of data, and return the number of bytes filled.
 */
unsigned char *generadata(int dcmd, const unsigned char* msg, unsigned char *data, size_t *length)
{
	if(dcmd != REPORT && dcmd != HEAERBEAR) {
		errno = EINVAL;
		return NULL;
	}

	struct interaction interac;
	interac.header = HEADER;
	interac.option = version(0);
	interac.cmd = CMD;
	interac.tail = TAIL;

	switch(dcmd) {
	case REPORT:
		*(interac.data) = REPORT; 
		data = report(interac, msg, data, length);
		break;
	case HEAERBEAR:
		*(interac.data) = HEAERBEAR; 
		data = heartbeat(interac, data, length);
		break;
	}
	return data;
}

/*
 * interac: protocol struct
 * data: save the generated
 * length: length of data
 */
static unsigned char *heartbeat(struct interaction interac,unsigned char *data, size_t *length)
{
	interac.length[0] = 0x0;
	interac.length[1] = 0x03;
	interac.crc = cal_crc(interac);
	return getdata(interac, data, length);
}

/*
 * interac: protocol struct
 * msg: the data for send
 * data: save the generated
 * length: length of data
 */
static unsigned char *report(struct interaction interac, const unsigned char *msg, unsigned char *data, size_t *length)
{
	int i;
	int len = strlen(msg);
	for(i = 0; i < len; i++) {
		*(interac.data + 1 + i) =*(msg++);
	}
	*(interac.data + len + 1) = '\0';
	//	memcpy(interac.data + 1, msg, strlen(msg));

	unsigned int n = strlen(interac.data) + 2; //+2:option, cmd
	interac.length[0] = (n >> (sizeof(byte) * 8) & 0xFF);
	interac.length[1] = (n & 0xFF);
	interac.crc = cal_crc(interac);

	return getdata(interac, data, length);
}

/*
 *
 * interac: protocol struct
 *
 */
static unsigned char cal_crc(struct interaction interac)
{
	int dlen = strlen(interac.data);
	unsigned char tocrc[ 4 + dlen];
	tocrc[0] = interac.length[0];
	tocrc[1] = interac.length[1];
	tocrc[2] = interac.option;
	int i;
	int len = strlen(interac.data);
	for(i = 0; i < len; i++) {
		*(tocrc + 3 + i) = *(interac.data + i);
	}

	return (cal_crc8_table(tocrc, strlen(interac.data) + 4));
}

/*
 * interac: protocol struct
 * data: save the generated data
 * length: length of data
 */
static unsigned char *getdata(struct interaction interac, unsigned char *data, size_t *length)
{
	unsigned char rsc[strlen(interac.data) + 6];
	rsc[0] = interac.length[0];
	rsc[1] = interac.length[1];
	rsc[2] = interac.option;
	rsc[3] = interac.cmd;
	int i;
	int dlen = strlen(interac.data);
	for(i = 0; i< dlen; i++) {
		*(rsc + 4 + i) = *(interac.data + i);
	}
	*(rsc + dlen + 4) = interac.crc;
	size_t len = dlen + 5;
	escape(rsc, data + 1, &len);

	*data = interac.header;
	*(data + len + 1) = interac.tail;
	*length = len + 2;
	return data;
}


int resolve(char *dstptr, const char *rscptr, size_t *len)
{
	if(dstptr == NULL || rscptr == NULL ) {
//|| *rscptr != 0xAA || *(rscptr + *len - 1) != 0x55
		printf("error\n");
		return -1;
	}
	byte rlen[2];
	rlen[0] = *(rscptr + 1);
	rlen[1] = *(rscptr + 2);

	int length = 0;
	length |= ((rlen[0] << sizeof(byte) * 8) | rlen[1]);
	byte option = *(rscptr + 3);
	byte cmd = *(rscptr + 4);
	byte crc = *(rscptr + *len - 2);
//	byte crc = *(rscptr + 20);

	int i;
	switch(*(rscptr + 5)) {
	case REPORT:
		for(i = 0; i< length - 3; i++) {
			*(dstptr++) = *(rscptr + 6 + i);
		}
		*len = length - 3;
		*(dstptr) = '\0';
		break;
	case HEAERBEAR:
		for(i = 0; i< *len; i++) {
			*(dstptr++) = *(rscptr + 5 + i);
		}
//		memcpy(dstptr, rscptr, *len);
		break;
	default:
		printf("unknow type\n");
		return -1;
	}
	return (*(rscptr + 5));
}



