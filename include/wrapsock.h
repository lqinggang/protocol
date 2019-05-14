#ifndef PROTOCOL_INCLUDE_WRAPSOCK_H
#define PROTOCOL_INCLUDE_WRAPSOCK_H

#include <netinet/in.h>

#define SERVERADDRESS   "127.0.0.1" 
#define SERVERPORT      12345
#define MAXLINE         1024
#define BACKLOG         16

extern int Socket(int domain, int type, int protocol);

extern void Inet_pton(int domain, const char *ptr, void *addrptr);
extern void Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);
extern ssize_t Send(int sockfd, const void *ptr, size_t len, int flags);
extern ssize_t Write(int sockfd, const void *ptr, size_t len);

extern int Bind(int sockfd, struct sockaddr *addr, socklen_t addrlen);
extern int Listen(int sockfd, int backlog);
extern int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

#endif  /* PROTOCOL_INCLUDE_WRAPSOCK_H */
