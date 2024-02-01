/* 4(b) date and time echo SERVER */

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

#define MAX_MESSAGE_SIZE 100

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char message[MAX_MESSAGE_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Could not create socket");
        exit(0);
    }

    // Prepare the sockaddr_in structure
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("Bind failed");
        exit(0);
    }

    // Listen
    listen(server_socket, 3);

    printf("Server listening on port 8888...\n");

    // Accept incoming connections
    int client_address_size = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&client_address_size);
    if (client_socket < 0) {
        printf("Accept failed");
        exit(0);
    }

    printf("Client connected\n");

    // Receive and send data
    while (1) {
        // Receive message from client
        memset(message, 0, sizeof(message));
        if (recv(client_socket, message, sizeof(message), 0) < 0) {
            printf("Receive failed");
            exit(EXIT_FAILURE);
        }

        // Print received message
        printf("Received message from client: %s\n", message);

        // Get current date and time
        time_t now;
        time(&now);
        strcpy(message, ctime(&now));

        printf("server's date and time: %s\n",message);

        // Send date and time to client
        if (send(client_socket, message, strlen(message), 0) < 0) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
        break;
    }

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}

=========================================================

/* 4(b) date and time echo CLIENT */

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

#define MAX_MESSAGE_SIZE 100

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char message[MAX_MESSAGE_SIZE], server_reply[MAX_MESSAGE_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);

    // Connect to remote server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("Connect failed");
        exit(0);
    }

    printf("Connected to server\n");

    while (1) {
        /*fgets(message, sizeof(message), stdin);*/
         // Send and receive data
        /* printf("Enter date and time: "); */
         time_t now;
         time(&now);
         strcpy(message, ctime(&now));

         printf("client's date and time: %s\n",message);
        
        // Send date and time to server
        if (send(client_socket, message, strlen(message), 0) < 0) {
            printf("Send failed");
            exit(0);
        }
    
        // Receive message from server
        memset(server_reply, 0, sizeof(server_reply));
        if (recv(client_socket, server_reply, sizeof(server_reply), 0) < 0) {
            printf("Receive failed");
            exit(0);
        }

        // Print received message
        printf("Server reply: %s\n", server_reply); 
        break;
    }

    // Close socket
    close(client_socket);

    return 0;
}
