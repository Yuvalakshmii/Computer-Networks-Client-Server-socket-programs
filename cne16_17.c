write a c program to do file handling using socket (server and client). 
  output:-
In client side: 
Enter the file name: sample
Number of words received from server: 10
On server side:
Name of the file opened: sample
number of words: 10
Overall count the number of words in opened (file.txt) and print it on server side as well as transmit it to the client side and print it

/* S E R V E R */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080
#define MAXLINE 1024

// Function to count words in a file
int count_words(FILE *fp) {
    int word_count = 0;
    char ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (isspace(ch)) {
            word_count++;
        }
    }

    // Account for the last word without trailing space
    if (ch != EOF) {
        word_count++;
    }

    return word_count;
}

int main() {
    int sockfd, newsockfd, valread;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAXLINE];
    int n;
    char filename[MAXLINE];
    FILE *fp;
    int total_words = 0;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // Accept a connection from a client
        newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &n);
        if (newsockfd < 0) {
            perror("accept failed");
            continue; // Try again for the next connection
        }

        // Receive the filename from the client
        valread = read(newsockfd, filename, MAXLINE);
        if (valread < 0) {
            perror("read failed");
            close(newsockfd);
            continue; // Try again for the next connection
        } else if (valread == 0) {
            printf("Client disconnected abruptly\n");
            close(newsockfd);
            continue; // Try again for the next connection
        }

        // Get the client's IP address and print it
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(cliaddr.sin_addr), client_ip, sizeof(client_ip));
      

        // Open the file on the server in read mode
        fp = fopen(filename, "r");
        if (fp == NULL) {
            printf("Error opening file: %s\n", filename);
            send(newsockfd, "Error opening file", strlen("Error opening file"), 0);
            close(newsockfd);
            continue; // Try again for the next connection
        }

        // Count the words in the file and send the total count to the client
        total_words = count_words(fp);
        sprintf(buffer, "%d", total_words);
        printf("Number of words : %d\n", total_words);
        send(newsockfd, buffer, strlen(buffer), 0);

        // Close the file and the connection
        fclose(fp);
        close(newsockfd);
    }

    return 0;
}
===============================================================================
/* C L I E N T */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    int valread;
    struct sockaddr_in servaddr;
    char buffer[MAXLINE];
    char filename[MAXLINE];
    int total_words;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);


    // Convert the IP address from string to binary
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        perror("Invalid IP address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // Get the filename from the user
    printf("Enter the filename: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        perror("fgets failed");
        exit(EXIT_FAILURE);
    }
    filename[strcspn(filename, "\n")] = 0; // Remove trailing newline

    // Send the filename to the server
    if (send(sockfd, filename, strlen(filename), 0) < 0) {
        perror("send failed");
        exit(EXIT_FAILURE);
    } 

    // Receive the total word count from the server
    valread = read(sockfd, buffer, MAXLINE);
    if (valread < 0) {
        perror("read failed");
        exit(EXIT_FAILURE);
    } else if (valread == 0) {
        printf("Server disconnected abruptly\n");
        exit(EXIT_FAILURE);
    }

    // Convert the received string to integer
    if (sscanf(buffer, "%d", &total_words) != 1) {
        perror("sscanf failed");
        exit(EXIT_FAILURE);
    }

    // Print the total word count
    printf("Number of words received from server: %d\n", total_words);

    // Close the socket
    close(sockfd);

    return 0;
}

