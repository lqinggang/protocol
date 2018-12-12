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

	char msg[MAXLINE];
	size_t n;
	printf("%% ");
	while(fgets(msg, MAXLINE, stdin) != NULL) {
		size_t length = strlen(msg);
		psend(REPORT, sockfd, msg, length - 1, 0); //-1: not included \n
		printf("%% ");
		fflush(NULL);
	}
	close(sockfd);

	exit(0);
}
