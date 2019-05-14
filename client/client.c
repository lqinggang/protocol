#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "wrapsock.h"
#include "crc.h"
#include "escape.h"
#include "protocol.h"

int sockfd; 
static int interval = 10;
pthread_mutex_t interval_lock= PTHREAD_MUTEX_INITIALIZER;

static void
sig_alrm(int signo)
{
	psend(HEARTBEAT, sockfd, NULL, 0, 0); //send heartbeat package
	alarm(interval);

	pthread_mutex_lock(&interval_lock);
	interval += interval / 2;
	if(interval > 90) 
	{
		kill(getpid(), SIGPIPE);	
	}
	pthread_mutex_unlock(&interval_lock);
}

static void 
sig_pipe(int signo)
{
	fprintf(stderr, "the server has been disconnected\n");	
	close(sockfd);
	exit(0);
}

static void *
fn_heartbeat(void *arg)
{
    int sockfd;
    sockfd = *(int *)arg;
	int len = MAXLINE;
	char recbuff[len * 2];
    memset(recbuff, 0, sizeof(recbuff));
	while(1)
    {
		ssize_t n = 0;
		if((n = recv(sockfd, recbuff, len, 0)) < 0)  // received data 
        {
			perror("recv error");
		}
        else if(n > 0) 
        {
			int type = resolve(NULL, recbuff, &n); //resolve the message
			if(type == HEARTBEAT) 
            {
				pthread_mutex_lock(&interval_lock);
				interval = 10;
				pthread_mutex_unlock(&interval_lock);
			}
		}
	}
	pthread_exit(NULL);
}

int
main(void)
{
//	//0. heartbeat package sent every 15 seconds 
//	signal(SIGALRM, sig_alrm);
//	alarm(interval);
//
//	signal(SIGPIPE, sig_pipe);

	//1. create a socket
	if((sockfd = Socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
		printf("socket error\n");
		exit(1);
	}

	//2. initialize the server address
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr)); //clear
	servaddr.sin_family = AF_INET;//ipv4
	servaddr.sin_port = htons(SERVERPORT); 
	servaddr.sin_addr.s_addr = inet_addr(SERVERADDRESS);
    //3. connect to the server
	Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)); 

	pthread_t tid;
	pthread_create(&tid, NULL, fn_heartbeat, &sockfd); // receive heartbeat packet

	//4. get message from stdin
	char msg[MAXLINE];
	printf("%% "); //prompt: %
	while(fgets(msg, MAXLINE, stdin) != NULL) 
    {  
		size_t length = strlen(msg);
		//5. send the message to the server
		psend(REPORT, sockfd, msg, length, 0);
		printf("%% ");
		fflush(NULL);
	}
	close(sockfd);

	exit(0);
}
