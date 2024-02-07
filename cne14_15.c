/* write a c program for character encoding using socket. first connect client and sever. secondly, in client side it should take input data example: SCOPE, then in server side it should get the received data from client "SCOPE". 3rd thing is the data should be encoded in server side by replacing the received data from client with third letter from the received letter. example: received from client: SCOPE; encoded data: VFRSH. 4th thing is the encoded data in server side should be received by client and print it. */

/* server */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>


#define PORT 8080
#define MAX_BUFFER_SIZE 1024

char encodeCharacter(char c) {
    // Replace the character with the third letter from it
    if (c >= 'A' && c <= 'Z') {
        c += 3;
        if (c > 'Z') {
            c -= 26;
        }
    }
    return c;
}

int main() {
    int serverSocket, newSocket, valread;
    struct sockaddr_in serverAddr;
    int addrLen = sizeof(serverAddr);
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket to a specific address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(serverSocket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept a new connection
    if ((newSocket = accept(serverSocket, (struct sockaddr *)&serverAddr, (socklen_t *)&addrLen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive data from the client
    valread = fread(newSocket, buffer, MAX_BUFFER_SIZE);
    printf("Received Data from Client: %s\n", buffer);

    // Encode the data
    for (int i = 0; i < strlen(buffer); i++) {
        buffer[i] = encodeCharacter(buffer[i]);
    }

    // Send the encoded data back to the client
    send(newSocket, buffer, strlen(buffer), 0);
    printf("Encoded Data Sent to Client: %s\n", buffer);

    return 0;
}

=========================================================================================
/*client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>


#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int clientSocket, valread;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Create a socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Take input data from the user
    printf("Enter data: ");
    fgets(buffer, MAX_BUFFER_SIZE, stdin);

    // Send the data to the server
    send(clientSocket, buffer, strlen(buffer), 0);

    // Receive the encoded data from the server
    valread = read(clientSocket, buffer, MAX_BUFFER_SIZE);
    printf("Received Encoded Data from Server: %s\n", buffer);

    return 0;
}

