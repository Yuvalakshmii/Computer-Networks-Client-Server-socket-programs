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
