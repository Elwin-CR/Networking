#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h> // For strcmp

#define PORT 8080
#define MAX 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buff[MAX];

// Socket creation
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Socket creation failed!!\n");
        return -1;
    } else {
        printf("Socket created successfully\n");
    }

// Server address setup
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Connect to localhost

    while (1) {
        // Send message to the server
        bzero(buff, sizeof(buff));
        printf("Enter a message to send to the server: \n");
        fgets(buff, sizeof(buff), stdin); // Using fgets to safely take input
        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
        printf("Message sent to server: %s\n", buff);

        // If the client sends "exit", break the loop
        if (strcmp(buff, "exit\n") == 0) {
            printf("Client has exited. Closing connection...\n");
            break;
        }

        // Read the server's response
        bzero(buff, sizeof(buff));

        socklen_t len = sizeof(servaddr);  // Length of the server address
        recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&servaddr, &len);  // Receive the message from the server
        printf("From server: %s\n", buff);  // Display the message received from the server
    }

    // Close the connection
    close(sockfd);
    printf("Connection closed with server.\n");

    return 0;
}

