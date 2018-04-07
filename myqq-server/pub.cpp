/*
 * pub.cpp
 *
 *  Created on: 2013年12月27日
 *      Author: zhujy
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include "pub.h"
#include "work.h"

void setdaemon()
{
	pid_t pid, sid;
	pid = fork();
	if (pid < 0)
	{
		printf("fork failed %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (pid > 0)
	{
		exit(EXIT_SUCCESS);
	}

	if ((sid = setsid()) < 0)
	{
		printf("setsid failed %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/*
	 if (chdir("/") < 0)
	 {
	 printf("chdir failed %s\n", strerror(errno));
	 exit(EXIT_FAILURE);
	 }
	 umask(0);
	 close(STDIN_FILENO);
	 close(STDOUT_FILENO);
	 close(STDERR_FILENO);
	 */
}

void catch_Signal(int Sign)
{
	switch (Sign)
	{
	case SIGINT:
		//printf("signal SIGINT\n");
		break;
	case SIGPIPE:
		//signal1(SIGPIPE, catch_Signal);
		break;
	}
}

int signal1(int signo, void (*func)(int))
{
	struct sigaction act, oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	return sigaction(signo, &act, &oact);
}

int socket_create(int port)//创建参数port指定端口号的server端socket
{
	int st = socket(AF_INET, SOCK_STREAM, 0);//创建TCP Socket
	int on = 1;
	if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		printf("setsockopt failed %s\n", strerror(errno));
		return 0;
	}
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)
	{
		printf("bind port %d failed %s\n", port, strerror(errno));
		return 0;
	}
	if (listen(st, 300) == -1)
	{
		printf("listen failed %s\n", strerror(errno));
		return 0;
	}
	return st;//返回listen的socket描述符
}

