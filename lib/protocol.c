#include "protocol.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include "crc.h"
#include "escape.h"
#include "wrapsock.h"


/*
 * calculate the crc value of  (length, option, cmd and data)
 * calculate the crc value for length[2] + option 
 * +cmd + data
 * interac: protocol struct
 *
 */
static char
cal_crc(struct interaction interac)
{
	int dlen = strlen(interac.data);
	char tocrc[ 4 + dlen];      // +4: length[2] + option + cmd
	memset(tocrc, 0, sizeof(tocrc));
	tocrc[0] = interac.option;
	tocrc[1] = interac.length[0];
	tocrc[2] = interac.length[1];
	tocrc[3] = interac.cmd;
	int i;
	//int len = strlen(interac.data); 
	for(i = 0; i < dlen; i++) 
    {
		*(tocrc + 4 + i) = *(interac.data + i); //+4: length[2] + option + cmd
	}

	return (cal_crc8_table(tocrc, strlen(interac.data) + 4)); //get the crc value
}

/*
 * generate protocol package
 * generate message
 * interac: protocol struct
 * data: save the generated data
 * length: length of data
 */
static char *
getdata(struct interaction interac, char *data, size_t *length)
{
	char rsc[strlen(interac.data) + 6]; //+6: length[2] + option + cmd + crc
	memset(rsc, 0, sizeof(rsc));
	rsc[0] = interac.option;
	rsc[1] = interac.length[0];
	rsc[2] = interac.length[1];
	rsc[3] = interac.cmd;

	int i;
	int dlen = strlen(interac.data);
	for(i = 0; i< dlen; i++) 
    {
		*(rsc + 4 + i) = *(interac.data + i); //+4: length[2] + option + cmd
	}

	/* 0        8        16      24       32
	 * |--------|--------|--------|--------|
	 * |length0 |length1 | option |  cmd   |
	 * |--------|--------|--------|--------|
	 * |    data: dlen  bytes     |     <- + - crc
	 * |--------|--------|--------|--------|
	 */
	*(rsc + dlen + 4) = interac.crc; //+4: length[2] + option + cmd 
	size_t len = dlen + 5;

	/*
	 * escape for  length[2] + option + cmd + data,
	 * the escaped data is padded to data, the length
	 * of the data after the escape is passed back 
	 * through len.
	 * +1: the first character is header, don't need to esacape
	 */
	escape(rsc, data + 1, &len); 

	*data = interac.header;
	*(data + len + 1) = interac.tail;
	*length = len + 2; //+2: header + tail
	return (data);
}

/*
 * generate date message
 * interac: protocol struct
 * msg: the data for send
 * data: save the generated
 * length: length of data
 */
static char *
report(struct interaction interac, const char *msg, char *data, size_t *length)
{
	int i;
	int len = *length;
	for(i = 0; i < len; i++) 
    {
		/* append  (msg) to the data segment 
		 *	+1: the first character is REPORT/HEARTBEAT, see generadata() methods
		 */
		*(interac.data + 1 + i) =*(msg++); 
	}
	*(interac.data + len + 1) = '\0'; 

	 int n = strlen(interac.data) + 2;  //+2:option and cmd

	interac.length[0] = (n >> (sizeof(byte) * 8) & 0xFF); 
	interac.length[1] = (n & 0xFF);

	interac.crc = cal_crc(interac);  //calculate the crc value

	return (getdata(interac, data, length));  //get all message 
}

/*
 * generate heartbeat package
 * interac: protocol struct
 * data: save the generated
 * length: length of data
 */
static char *
heartbeat(struct interaction interac, char *data, size_t *length)
{
	interac.length[0] = 0x0;
	interac.length[1] = 0x03;
	interac.crc = cal_crc(interac); //calculate the crc value
	return (getdata(interac, data, length)); //get all message 
}

/*
 * get the date
 * interac: protocol struct
 * msg: the data for send
 * data: save the generated
 * length: length of data
 */
static char *
get_data(struct interaction interac, const char *msg, char *data, size_t *length)
{
	int i;
	int len = *length;
	for(i = 0; i < len; i++) 
    {
		/* append  (msg) to the data segment 
		 *	+1: the first character is REPORT/HEARTBEAT, see generadata() methods
		 */
		*(interac.data + 1 + i) =*(msg++); 
	}
	*(interac.data + len + 1) = '\0'; 

	 int n = strlen(interac.data) + 2; //+2:option and cmd

	interac.length[0] = (n >> (sizeof(byte) * 8) & 0xFF); 
	interac.length[1] = (n & 0xFF);

	interac.crc = cal_crc(interac); //calculate the crc value

	return (getdata(interac, data, length)); //get all message 
}

/*
 * generate data that needs to be send
 * dcmd: the data command type
 * msg: the message, need to send
 * data: save the generated data
 * length: length of data, and return the number of bytes filled.
 */
static char *
generadata(int dcmd, const void *msg, char *data, size_t *length)
{
	/* wrong data segment command type */
	if(dcmd != REPORT && dcmd != HEARTBEAT) 
    { 
		errno = EINVAL;
		return (NULL);
	}

	char *sendmsg = (char *)msg;
	struct interaction interac;
	interac.header = HEADER;
	interac.option = version(0);
	interac.cmd = CMD;
	interac.tail = TAIL;

	switch(dcmd)
    {
	case REPORT:
		*(interac.data) = REPORT; 
		break;
	case HEARTBEAT:
		*(interac.data) = HEARTBEAT; 
		break;
    default:
        break; 
	}

    data = get_data(interac, sendmsg, data, length); //get report message
	return (data);
}

static ssize_t
recv_timeout(int sockfd, void *buf, size_t len)
{
	/*
	 *  Try to receive data
	 */
    int numsec;
    ssize_t receive_length;
	for(numsec = 1; numsec <= TIMEOUT; numsec <<= 1) 
    {
		if((receive_length = recv(sockfd, buf, len, MSG_DONTWAIT)) > 0) 
        {
            return (receive_length);
		}
        else if (receive_length == 0)  /* the peer has performed an orderly shutdown. */
        {
            break;
        }

		/*
		 * Delay defore trying again.
		 */
		if(numsec <= TIMEOUT / 2)
				sleep(numsec);
	}
    errno = ETIMEDOUT;
    return (-1);
}


/*
 * resolve data of receive the message
 */
int
resolve(char *dstptr, const char *rscptr, ssize_t *len)
{
	if(rscptr == NULL || *(unsigned char *)rscptr != HEADER || *(unsigned char *)(rscptr + *len - 1) != TAIL)
    {
		errno = EINVAL;
		perror("resolve error");
		return (-1);
	}

	/*     byte    byte    byte    byte
	 *  0		8		16		24		32
	 *  |-------|--------|-------|-------|
	 *  |  head | [0] length [1] | option|
	 *  |-------|--------|-------|-------|
	 *  |  cmd  |  dcmd  |	     |  tail |
	 *  |-------|--------|-------|-------|
	 *          |  data: N byte  |
	 */
	byte rlen[2];
	rlen[0] = *(rscptr + 2); 
	rlen[1] = *(rscptr + 3);

	int length = 0;
	/* byte length[2] to  int length */
	length |= ((rlen[0] << sizeof(byte) * 8) | rlen[1]);

//	byte option = *(rscptr + 1);
//	byte cmd = *(rscptr + 4);
//	byte crc = *(rscptr + *len - 2);

	char rescptr[*len + 1];
	memset(rescptr, 0, *len + 1);

	size_t reslength = *len - 2;
	reescape(rscptr + 1, rescptr, &reslength);
	if(cal_crc8_table(rescptr, reslength - 1) != (byte)*(rescptr + reslength - 1)) 
    {
		fprintf(stderr,"data error\n");
		return (-1);
	}
	if(dstptr == NULL) 
    {
		return (*(rescptr + 4));
	} 
    else
    {
		memset(dstptr, 0, strlen(dstptr));
	}

	int i;
	switch(*(rescptr + 4))  // *(rescptr + 4)  is the dcmd
    {
	case REPORT:
	case HEARTBEAT:

		/*
		 * -3:  option + cmd + dcmd
		 * +5: length[2] + option + cmd + dcmd
		 */
		for(i = 0; i< length - 3; i++)
        { 
			/*
			 *		 *(dstptr++) 
			 * equivalent to:
			 *		 dstptr++;
			 *		 *dstptr = *(resptr + 5 + i);
			 */
			*(dstptr++) = *(rescptr + 5 + i);
		}
		break;
//	case HEARTBEAT:
//		for(i = 0; i< *len; i++) 
//        {
//			*(dstptr++) = *(rescptr + 4 + i);
//		}
//		break;
	default:
		printf("Unknow type\n");
		return (-1);
	}
	*len = length - 3; //-3:  option + cmd + dcmd
	*(dstptr) = '\0';
	return (*(rscptr + 5));
}

/*
 * send message 
 * dcmd: data command 
 * sockfd: socket 
 * buf: the message need to be send 
 * len: the length of the buf
 * flags: see the send() method
 */
ssize_t 
psend(int dcmd, int sockfd, const void *buf, size_t len, int flags)
{
	/*
	 * not send when the length is less than 
	 * or equal to 0
	 */
//	if(--len <= 0)   //-1: not included \n
//		return (0); 

	/* wrong data segment command type */
	if(dcmd != REPORT && dcmd != HEARTBEAT)
    {
		errno = EINVAL;
		perror("Unknow type");
		return (-1);
	}

	char *data = NULL;
	if((data = (char *)malloc(sizeof(char) * len * 2)) == NULL)
    {
		printf("psend error:no enough space\n");
		return (-1);
	}
	memset(data, 0, strlen(data)); //clear

	size_t length = len;

	/*
	 * get the message, the data will be fill by generated 
	 * message, and the length of the data after the escape
	 *is passed back through length.
	 */
	generadata(dcmd, buf, data, &length); 

	int err;
	int maxfdl;
	fd_set wset;
	struct timeval time;
	time.tv_sec = 0; //not wait
	time.tv_usec = 0;
	FD_ZERO(&wset);
	FD_SET(sockfd, &wset);
	maxfdl = sockfd + 1;
	if(select(maxfdl, NULL, &wset, NULL, &time) < 0) 
    {
		perror("select error");
		err = errno;
	}
	ssize_t n = 0;
	if(FD_ISSET(sockfd, &wset)) 
    {
		n = Send(sockfd, data, length, flags); //send the message 
	} 
    else
    {
		fprintf(stderr, strerror(err));
	}
	free(data);
	return (n);
}

/*
 * receive message 
 * sockfd: socket 
 * buf: the received data
 * len: the length of the buf
 * flags: see the send() method
 */
ssize_t
precv(int sockfd, void *buf, size_t len, int flags)
{
    if (buf == NULL)
    {
        return (-1);
    }
	char recbuff[len * 2];
	memset(recbuff, 0, len * 2);

	ssize_t n = 0;
	if((n = recv_timeout(sockfd, recbuff, len)) < 0)  // received data 
    { 
        if (errno == ETIMEDOUT)
        {
            return (-1);
        }
		perror("recv error");
		return (errno);
	}

	char data[len];
    memset(data, 0, sizeof(data));  //clear
	if(n > 0) 
    {
		int type;
		type = resolve(data, recbuff, &n); //resolve the message
		if(type == REPORT && n > 0) 
        {
			strncpy(buf, data, n);
			return (n);
		}
        else if(type == HEARTBEAT)
        {
			/*
			 * when receiving a heartbeat packet, turn around
			 */
			strncpy(buf, data, n);
			return (psend(HEARTBEAT, sockfd, HEARTRESPOND, strlen(HEARTRESPOND), flags));
		}
        else
        {
			return (-1);
		}

		fflush(NULL);
	}
    return (0);
}
