/* write a c program for character encoding using socket. first connect client and sever.
secondly, in client side it should take input data ,example: SCOPE, then in server side it should receive the data from client i.e., "SCOPE". 
The work in server side is to encode the data recieved by replacing the data with it corresponding third letter from the received data's letter. 
example: received from client: SCOPE; encoded data: VFRSH. 
4th thing is the encoded data in server side should be transmitted to client and client should print it. */
 
// server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 100

char encodeData(char ch, int shift) {
    if (ch >= 'A' && ch <= 'Z') {
        ch = ((ch - 'A' + shift) % 26) + 'A';
    } else if (ch >= 'a' && ch <= 'z') {
        ch = ((ch - 'a' + shift) % 26) + 'a';
    }
    return ch;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char receivedData[MAX_BUFFER_SIZE];
    char encodedData[MAX_BUFFER_SIZE];
    int shift = 3; // Define the shift value

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

    memset(receivedData, 0, sizeof(receivedData));
    read(new_socket, receivedData, sizeof(receivedData));
    printf("Data received from client: %s\n", receivedData);

    for (int i = 0; receivedData[i] != '\0'; i++) {
        encodedData[i] = encodeData(receivedData[i], shift);
    }
    encodedData[strlen(receivedData)] = '\0'; // Add null character at the end of the encoded data

    send(new_socket, encodedData, strlen(encodedData), 0);
    printf("Encoded data: %s\n", encodedData);

    close(new_socket);
    close(server_fd);

    return 0;
}

====================
// client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 100

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char data[MAX_BUFFER_SIZE];
    char receivedData[MAX_BUFFER_SIZE];
    int shift = 3; // Define the shift value

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation failed\n");
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

    printf("Enter data to send: ");
    fgets(data, sizeof(data), stdin);
    data[strcspn(data, "\n")] = '\0'; // Remove the newline character from input

    send(sock, data, strlen(data), 0);

    memset(receivedData, 0, sizeof(receivedData));
    read(sock, receivedData, sizeof(receivedData));
    printf("Encoded data received from server: %s\n", receivedData);

    close(sock);

    return 0;
}

