#include "wrapsock.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int
Socket(int domain, int type, int protocol)
{
	int sockfd;
	if((sockfd = socket(domain, type, protocol)) < 0)
    {
		perror("socket error");	
		return -1;
	}
	return (sockfd);
}

void
Inet_pton(int domain, const char *ptr, void *addrptr)
{
	int n;
	if((n = inet_pton(domain, ptr, addrptr)) <= 0)
    {
		if(n == 0)
        {
			printf("not in presentation address\n");
		}
        else  
        {
			perror("inet_pton error");
		}
		exit(1);
	}
}

void
Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen)
{
	int n = -1;
	if((n = connect(sockfd, servaddr, addrlen)) < 0) 
    {
		perror("conect error");
        exit(-1);
	}
}


ssize_t
Send(int sockfd, const void *ptr, size_t len, int flags)
{
	ssize_t n;
	if((n = send(sockfd, ptr, len, flags)) < 0)
    {
		perror("send error");
	}
	return n;
}

ssize_t
Write(int sockfd, const void *ptr, size_t len) 
{
	ssize_t n;
	if((n = write(sockfd, ptr, len)) < 0) 
    {
		perror("send error");
	}
	return n;
}

int
Listen(int sockfd, int backlog)
{
	if(listen(sockfd, backlog) < 0) 
    {
		perror("listen error");
		return -1;
	}
	return 0;
}

int
Bind(int sockfd, struct sockaddr *addr, socklen_t addrlen)
{
	int on = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		perror("bind error: setsockopt error");
		if(close(sockfd ) < 0)
        {
			perror("close error");
		}
		exit(-1);
	}

	if(bind(sockfd, addr, addrlen) < 0)
    {
		perror("bind error");
		return -1;
	}
	return 0;
}

int
Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) 
{
	int acceptfd;
	if((acceptfd = accept(sockfd, addr, addrlen)) < 0)
    {
		perror("accept error");
		return -1;
	}
	return acceptfd;
}
