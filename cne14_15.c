/* write a c program for character encoding using socket. first connect client and sever.
secondly, in client side it should take input data ,example: SCOPE, then in server side it should receive the data from client i.e., "SCOPE". 
The work in server side is to encode the data recieved by replacing the data with it corresponding third letter from the received data's letter. 
example: received from client: SCOPE; encoded data: VFRSH. 
4th thing is the encoded data in server side should be transmitted to client and client should print it. */
 
/* CRC server */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

void calculateCRC(char data[], char divisor[], char crc[]) {
    int dataLen = strlen(data);
    int divisorLen = strlen(divisor);
    
    // Append zeros to the data
    for (int i = 0; i < divisorLen - 1; i++) {
        data[dataLen + i] = '0';
    }

    // Perform division
    for (int i = 0; i < dataLen; i++) {
        if (data[i] == '1') {
            for (int j = 0; j < divisorLen; j++) {
                data[i + j] = (data[i + j] == divisor[j]) ? '0' : '1';
            }
        }
    }

    strncpy(crc, &data[dataLen], divisorLen - 1);
    crc[divisorLen - 1] = '\0';
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char data[100];
    char divisor[100];
    char crc[100];

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        printf("Socket creation failed\n");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("Socket binding failed\n");
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        printf("Listening failed\n");
        return -1;
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        printf("Acceptance failed\n");
        return -1;
    }
   
    read(new_socket, buffer, sizeof(buffer));
    sscanf(buffer, "%s\n%s", data, divisor);

    calculateCRC(data, divisor, crc);

    printf("Calculated CRC is: %s\n", crc);

    close(new_socket);
    close(server_fd);

    return 0;
}

====================
/* CRC client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char data[100];
    char divisor[100];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    printf("Enter the data: ");
    scanf("%s", data);

    printf("Enter the divisor: ");
    scanf("%s", divisor);

    char message[200];
    sprintf(message, "%s\n%s", data, divisor);

    send(sock, message, strlen(message), 0);

    printf("Data and divisor sent to the server\n");

    close(sock);

    return 0;
}
