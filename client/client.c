/****************************************
  > File Name: main.c
  > Author: lqinggang
  > Email: 1944058861@qq.com
  > Create Time: Sat 08 Dec 2018 01:08:45 PM CST
 ****************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "wrapsock.h"
#include "crc.h"
#include "escape.h"
#include "protocol.h"

#define ALRMTIME  30

int sockfd; 

static void sig_alrm(int signo);


int main(int argc,char *argv[])
{
	//0. heartbeat package sent every 15 seconds 
	signal(SIGALRM,sig_alrm);
	alarm(ALRMTIME);

	//1. create a socket
	if((sockfd = Socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		printf("socket error\n");
		exit(1);
	}

	//2. initialize the server address
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr)); //clear
	servaddr.sin_family = AF_INET;//ipv4
	servaddr.sin_port = htons(ServPort); 
	servaddr.sin_addr.s_addr = inet_addr(ServAddr);

	//3. connect to the server
	Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)); 

	//4. get message from stdin
	char msg[MAXLINE];
	printf("%% "); //prompt: %
	while(fgets(msg, MAXLINE, stdin) != NULL) {  
		size_t length = strlen(msg);
		//5. send the message to the server
		psend(REPORT, sockfd, msg, length - 1, 0); //-1: not included \n
		printf("%% ");
		fflush(NULL);
	}
	close(sockfd);

	exit(0);
}

static void sig_alrm(int signo)
{
	psend(HEARTBEAT, sockfd, NULL, 0, 0); //send heartbeat package
	alarm(ALRMTIME);
}
