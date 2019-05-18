#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "wrapsock.h"
#include "protocol.h"

//static void
//sig_chld(int signo)
//{
//	int status;
//	while(wait(-1, &status,WNOHANG) > 0);
//}


int
main(void)
{
	//1.server addr
	struct sockaddr_in servaddr;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //0.0.0.0
	servaddr.sin_port = htons(SERVERPORT);//12345
	servaddr.sin_family = AF_INET;

	//2.create a socket
	int listenfd;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	//3.bind
	Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	//4.listen
	Listen(listenfd, BACKLOG);

	while(1)
    {
		printf("wait for client....\n");

		//5.wait for client access
		int accfd;
		struct sockaddr addr;
		socklen_t addrlen;
		if ((accfd = Accept(listenfd, (struct sockaddr *) &addr, &addrlen)) < 0) 
        { 
			if (errno == EINTR)
            {
				continue;
            }
			else
            {
				close(listenfd);
				return -1;
			}
		}
        else
        {
			//get client info
			struct sockaddr_in cliaddr;
			socklen_t cliaddrlen = sizeof(cliaddr);
			if (getpeername(accfd, (struct sockaddr *) &cliaddr, &cliaddrlen) < 0)
            {
				printf("getpeername error.\n");
			}
            else
            {
				printf("connect by %s:%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
			}

			/*
			 * if fork() success, the parent and child process will wach have 
			 * an accfd and listenfd, so we should close the listenfd socket 
			 * of the child process, and the accfd socket of the parent process
			 */
			//6.create a process to receive data.
			pid_t pid;
			if ((pid = fork()) < 0)
            {
				perror("fork error: ");
				close(listenfd);
				close(accfd);
				return -1;
			}
            else if (pid == 0)  // first child
            {
				/*
				 * only keep the listenfd of the parent process
				 */
				close(listenfd); 

				/* that will be have two accfd socket,if success */
				pid_t pid2;
				if((pid2 = fork()) < 0)
                {
					perror("fork error: ");
					close(accfd);
					return -1;
				}
                else if (pid2 == 0)  // second child
                { 
					sleep(1);

					//7. receive messsage 
					ssize_t n;
					char buf[MAXLINE];
			        memset(buf, 0, MAXLINE);
					while ((n = precv(accfd, buf, MAXLINE, 0)) > 0) 
                    {
						fprintf(stdout, "recv: %s\n", buf);
						fflush(NULL);
						memset(buf, 0, MAXLINE);
					}  //end recv

					if (errno != EINTR && errno != EAGAIN)
                    {
						printf("disconnect by %s:%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
						fflush(stdout);
						close(accfd);
						exit(1);
					}
				} 

				/*
				 * first child 
				 */

				/*
				 * only keep the listenfd of the parent process
				 */
				close(accfd);

				/*
				 * make the parent process of the second
				 * second child process into init process
				 */
				exit(0); 

			} 

			/*
			 * parent of first child.
			 */

			/*
			 * only keep the accfd of the first child process
			 */
			close(accfd); 
			if (waitpid(pid, NULL, 0) != pid) 
            {
				printf("waitpid error\n");
			}

			//	/* wait for child process
			//	 * prevent zombie process
			//	 */
			//	if(signal(SIGCHLD, sig_chld) < 0) { 
			//		perror("SIGCHLD error");
			//	}
			//do something
		}
	}
	return 0;
}
