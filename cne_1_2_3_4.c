/* socket() cne1.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int s1, s2;
    s1=socket(AF_INET, SOCK_STREAM, 0);
    s2=socket(PF_INET, SOCK_DGRAM, 0);
    if (s1==-1 & s2==-1){
        printf("socket conn error!");
    }
    else{
        printf("socket conn successful!");
    }
    return 0;
}
===========================================
/* bind() cne2.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define PORTNO 2000

int main(){
	int s, i=PORTNO;
	struct sockaddr_in myaddr;
	if((s=socket(AF_INET, SOCK_STREAM,0))==-1){
		printf("socket error");
		exit(-1);
	}

	myaddr.sin_family=AF_INET;
	myaddr.sin_port=htons(PORTNO);
	myaddr.sin_addr.s_addr=INADDR_ANY;
	memset(&(myaddr.sin_zero),'\0',8);

	if (bind(s,(struct sockaddr *)&myaddr,sizeof(struct sockaddr))!=-1){
		printf("socket is binded at port %d\n", i);
	}
	else{
		printf("its not binded");
	}
}

======================================

/* listen() cne3.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#define PORTNO 2000
#define BACKLOG 12

int main()
{
	int fd;  /*file desciption*/
	struct sockaddr_in server;
	struct sockaddr_in client;

	int sin_size;
	int x;

	if((fd=socket(AF_INET,SOCK_STREAM,0))==-1){
		printf("socket() error\n");
		exit(-1);
	}

	server.sin_family=AF_INET;
	server.sin_port=htons(PORTNO);
	server.sin_addr.s_addr=INADDR_ANY;
	/* bzero(&(server.sin_size),8); */

	if(bind(fd,(struct sockaddr*)& server,sizeof(struct sockaddr))==-1){
		printf("bind() error\n");
		exit(-1);
	}

	x=listen(fd,BACKLOG);
	if(x==-1){
		printf("listen() error");
	}
	else{
		printf("server is in listening mode!\n");
	}

	close(fd);
}

====================================

/* accept() cne4.c */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 3550   /* Assigning Port numbers */
#define BACKLOG 2   /* Number of allowed connections */

int main()
{
    int fd, fd2;              /* file descriptors */
    struct sockaddr_in server;        /* server's address information */
    struct sockaddr_in client;        /* client's address information */
    int sin_size;
    
    if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 )  /* calls socket() */
    {
        printf("socket() error\n");
        exit(-1);
    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);    /* Remember htons() from "Conversions" section */
    server.sin_addr.s_addr = INADDR_ANY;  /* INADDR_ANY puts your IP address automatically */
    bzero(&(server.sin_zero),8);  /* zero the rest of the structure */
    
    if(bind(fd,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){ /* calls bind() */
        printf("bind() error\n");
        exit(-1);
    }
    
    if(listen(fd,BACKLOG) == -1)  /* calls listen() */
    {
        printf("listen() error\n");
        exit(-1);
    }
    
    printf("server is in accept mode\n");
    
    while(1)
    {
        sin_size=sizeof(struct sockaddr_in);
        if ((fd2 = accept(fd,(struct sockaddr *)&client,(socklen_t*)&sin_size))==-1){ /* calls accept() */
            printf("accept() error\n");
            exit(-1);
        }
        else
            printf("Server is in accept mode\n");
        
        printf("You got a connection from %s\n",inet_ntoa(client.sin_addr)); /* prints client's IP */
        close(fd2); /* close fd2 */
    }
    
    return 0;
}
