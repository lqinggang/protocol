/****************************************
  > File Name: server.c
  > Author: lqinggang
  > Email: 1944058861@qq.com
  > Create Time: Tue 11 Dec 2018 03:54:25 PM CST
 ****************************************/


#include <unistd.h>
#include <errno.h>
#include "wrapsock.h"
#include <signal.h>

static void sig_chld(int signo)
{
	int status;
	while(wait(-1, &status,WNOHANG) > 0);
}


int main(int argc,char *argv[])
{
	//1.server addr
	struct sockaddr_in servaddr;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(ServPort);
	servaddr.sin_family = AF_INET;

	//2.create a socket
	int listenfd;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	//3.bind
	Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));


	//4.listen
	Listen(listenfd, BACKLOG);

	//5.wait for client access
	while(1) {
		int accfd;
		struct sockaddr addr;
		socklen_t addrlen;
		printf("wait for client....\n");
		if((accfd = Accept(listenfd, (struct sockaddr *)&addr, &addrlen)) < 0) { 
			if(errno == EINTR)
				continue;
			else {
				close(listenfd);
				return -1;
			}
		} else {
			//get client info
			struct sockaddr_in cliaddr;
			socklen_t cliaddrlen = sizeof(cliaddr);
			if(getpeername(accfd,(struct sockaddr *)&cliaddr,&cliaddrlen) < 0) {
				printf("getpeername error.\n");
			} else {
				printf("connect by %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port );

			}
			//6.create a process to receive data.
			pid_t pid;
			if((pid = fork()) < 0) {
				perror("fork error: ");
				close(listenfd);
				close(accfd);
				return -1;
			} else if(pid == 0) { //child

				pid_t pid2;
				if((pid2 = fork()) < 0) {
					perror("fork error: ");
					close(listenfd);
					close(accfd);
					return -1;
				} else if(pid2 == 0) { // second child
					sleep(1);
					size_t n;
					char buf[MAXLINE];
					while((n = precv(accfd, buf, MAXLINE, 0)) > 0) {
						fprintf(stdout ,"recv: %s\n", buf);
						fflush(NULL);
					} //end recv
					if(errno != EINTR) {
						printf("disconnect by %s:%d\n",
								inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port );
						fflush(stdout);
						close(accfd);
						exit(1);
					}
				} else { //first child 
					exit(0);
				}

			} else { //parent
				if(signal(SIGCHLD, sig_chld) < 0) {
					perror("SIGCHLD error");
				}
				//do something
			}
		}
	}
	return 0;
}
