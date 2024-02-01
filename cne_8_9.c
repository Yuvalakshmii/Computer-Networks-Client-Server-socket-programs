/* TCP server chat cne8.c*/

#include<stdio.h>
 
#include<sys/types.h>
 
#include<netinet/in.h>
 
#include<sys/socket.h>
 
#include<string.h>
 
#include<unistd.h>
 
#include<stdlib.h>
 
#define PORT 4771
 
int main(int argc,char *argv[])
 
{
 
char buffer[100];
 
int sockfd,a,connfd,len,i=0;
 
struct sockaddr_in servaddr,cliaddr;
 
sockfd=socket(AF_INET,SOCK_STREAM,0);
 
if(sockfd==-1)
 
{
 
printf("Error creating socket\n");
 
exit(0);
 
}
 
printf("Server Socket Created Successfully.\n");
 
bzero((struct sockaddr *)&servaddr,sizeof(servaddr));
 
servaddr.sin_family=AF_INET;
 
servaddr.sin_port=htons(PORT);
 
servaddr.sin_addr.s_addr=INADDR_ANY;
 
if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
 
{
 
printf("Error in BIND function");
 
exit(0);
 
}
 
printf("Server Socket Binded.\n");
 
if((a=listen(sockfd,5))==-1)
 
{
 
printf("Error in LISTEN function");
 
exit(0);
 
}
 
printf("Server Socket Listened...\n");
 
len=sizeof(cliaddr);
 
if((connfd=accept(sockfd,(struct sockaddr *)&cliaddr,(socklen_t*)&len))<0)
 
{
 
printf("Error in ACCEPT");
 
exit(0);
 
}
 
do
 
{   
 
strcpy(buffer," ");
 
read(connfd,buffer,100);
 
printf("From client :%s",buffer);
 
if(strcmp(buffer,"bye\n")==0)
 
{
 
printf("from Client: %s",buffer);
 
goto stop;
 
}
 
printf("Server :");
 
fgets(buffer,sizeof(buffer),stdin);
 
write(connfd,buffer,100);
 
}
while(strcmp(buffer,"bye\n")!=0);
 
stop:
 
exit(0);
 
close(connfd);
 
return 0;
 
}
================================================
/* TCP client chat cne9.c*/

#include<stdio.h>
 
#include<sys/types.h>
 
#include<netinet/in.h>
 
#include<sys/socket.h>
 
#include<arpa/inet.h>
 
#include<string.h>
 
#include<unistd.h>
 
#include<stdlib.h>
#include<netdb.h>
 
#define PORT 4771
 
int main(int argc,char *argv[])
 
{
 
int sockfd;
 
struct sockaddr_in serv;
 
char buff[100];
 
sockfd=socket(AF_INET,SOCK_STREAM,0);
 
printf("Client Socket Created Successfully.\n");
 
memset(&serv,0,sizeof(serv));
 
serv.sin_family=AF_INET;
 
serv.sin_port=htons(PORT);
 
serv.sin_addr.s_addr=INADDR_ANY;
 
if (connect(sockfd,(struct sockaddr *)&serv, sizeof(serv))<0)
 
{
 
printf("error in connect");
 
exit(0);
 
}
 
printf("Client Socket with Server Successfully.\n");
 
do
 
{
 
strcpy(buff," ");
 
printf("Client. :");
 
fgets(buff,100,stdin);
 
write(sockfd ,buff,100);
 
if(strcmp(buff,"bye\n")==0)
 
{
 
printf("Client: %s",buff);		
 
goto stop;
 
}	
 
strcpy(buff," ");
 
read(sockfd,buff,sizeof(buff));
 
printf("From Server :%s\n",buff);
 
}
while(strcmp(buff,"bye\n")!=0);
 
stop:  
 
exit(0);
 
close(sockfd);
 
return 0;
 
}
