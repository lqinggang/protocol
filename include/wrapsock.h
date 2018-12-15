/****************************************
   > File Name: wrapsock.c
   > Author: lqinggang
   > Email: 1944058861@qq.com
   > Create Time: Sat 08 Dec 2018 01:09:57 PM CST
****************************************/


#ifndef WRAPSOCK_H
#define WRAPSOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ServAddr "192.168.10.253" 
#define ServPort 12345
#define MAXLINE 1024
#define BACKLOG  16

int Socket(int domain, int type, int protocol);

void Inet_pton(int domain, const char *ptr, void *addrptr);
void Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);
ssize_t Send(int sockfd, const void *ptr, size_t len, int flags);
ssize_t Write(int sockfd, const void *ptr, size_t len);

int Bind(int sockfd, struct sockaddr *addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

#endif

