/* 22MIA1040 server */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc,char **argv)

{

	char str[100];
	int s, comm_fd;
	struct sockaddr_in servaddr;

	s= socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(22000);

	bind(s, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(s, 10);
	comm_fd = accept(s, (struct sockaddr*) NULL, NULL);

	while(1)

	{
		bzero(str, 100);
		read(comm_fd ,str,100);
		printf("Echoing back - %s",str);
		write(comm_fd, str, strlen(str)+1);

}

}
