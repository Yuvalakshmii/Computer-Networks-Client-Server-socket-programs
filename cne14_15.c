/* write a c program for character encoding using socket. first connect client and sever.
secondly, in client side it should take input data ,example: SCOPE, then in server side it should receive the data from client i.e., "SCOPE". 
The work in server side is to encode the data recieved by replacing the data with it corresponding third letter from the received data's letter. 
example: received from client: SCOPE; encoded data: VFRSH. 
4th thing is the encoded data in server side should be transmitted to client and client should print it. */

/* server */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

char encodeData(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        ch = ((ch - 'A' + 3) % 26) + 'A';
    } else if (ch >= 'a' && ch <= 'z') {
        ch = ((ch - 'a' + 3) % 26) + 'a';
    }
    return ch;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char data[100];
    char encodedData[100];

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

    memset(data, 0, sizeof(data));
    read(new_socket, data, sizeof(data));

    for (int i = 0; i < strlen(data); i++) {
        encodedData[i] = encodeData(data[i]);
    }

    send(new_socket, encodedData, strlen(encodedData), 0);

    printf("Encoded data sent to client: %s\n", encodedData);

    close(new_socket);
    close(server_fd);

    return 0;
}

====================
/* client */

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

    send(sock, data, strlen(data), 0);

    char encodedData[100];
    memset(encodedData, 0, sizeof(encodedData));
    read(sock, encodedData, sizeof(encodedData));

    printf("Encoded data received from server: %s\n", encodedData);

    close(sock);

    return 0;
}

