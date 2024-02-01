/* 22MIA1040 server cne5.c */

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
======================================

/* 22MIA1040 client cne6.c */


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 22000

int main(int argc,char **argv)

{
	int s,n;
	char sendline[100];
	char recvline[100];
	struct sockaddr_in servaddr;

	s=socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(PORT);

	inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));

	connect(s,(struct sockaddr *)&servaddr,sizeof(servaddr));

	while(1)

	{
		bzero(sendline, 100);
		bzero(recvline, 100);

		fgets(sendline,100,stdin);

		write(s,sendline,strlen(sendline)+1);
		read(s,recvline,100);

		printf("%s",recvline);

	}
}
