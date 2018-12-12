/****************************************
  > File Name: main.c
  > Author: lqinggang
  > Email: 1944058861@qq.com
  > Create Time: Sat 08 Dec 2018 01:08:45 PM CST
 ****************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrapsock.h"
#include "crc.h"
#include "escape.h"
#include "protocol.h"

int main(int argc,char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr;

	if((sockfd = Socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		printf("socket error\n");
		exit(1);
	}

	char *addrbuf[sizeof(socklen_t)];
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(ServPort);
	servaddr.sin_addr.s_addr = inet_addr(ServAddr);

	Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	char *data = (char *) malloc(sizeof(char) * 100);
	size_t length;

	bzero(data,sizeof(data));
	//
	//	*data =  0xAA;
	//	*(data + 1) = 0;
	//	*(data + 2) = 0x06;
	//	*(data + 3) = 0;
	//	*(data + 4) = 0x04;
	//	*(data + 5) = 0x02;
	//	*(data + 6) = 'Y';
	//	*(data + 7) = 'e';
	//	*(data + 8) = 's';
	//	char *crc = (char *) malloc(sizeof(char) * 100);;
	//	int i;
	//	for(i = 0; i < 7; i++) {
	//		*(crc + i) = *(data + 1 + i);
	//	}
	//	*(data + 9) = cal_crc8_table(crc,7);
	//	*(data + 10) = 0x55;
	//	*length = 11;


	char *msg = " send to server           ";
//	if(argc >= 2) {
//		strcpy(msg, argv[1]);
//	}
	generadata(REPORT, msg, data, &length);
//	Send(sockfd, data, length, 0);
//	bzero(data,sizeof(data));
//	generadata(HEAERBEAR, "send to server", data, &length);
	Send(sockfd, data, length, 0);
//  Send(sockfd, msg, strlen(msg), 0);
	close(sockfd);
	free(data);
	exit(0);
}
