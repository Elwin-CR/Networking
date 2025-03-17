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
    struct sockaddr_in servaddr;
    char buff1[MAX];
    char buff2[MAX];
    char buff3[MAX];
    

// Socket creation
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) {
        printf("Socket creation failed!!\n");
        return -1;
    }
    else {
        printf("Socket created successfully\n");
    }

// Server address setup
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Connect to localhost

// Connect to the server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection to the server failed!!\n");
        close(sockfd); // Close the socket if connection fails
        return -1;
    }
    else {
        printf("Connected to server...\n");
    }

        // Send message to the server
        bzero(buff1,sizeof(buff1));
        bzero(buff2,sizeof(buff2));
        printf("Enter a number: \n");
        fgets(buff1, sizeof(buff1), stdin); // Using fgets to safely take input
        printf("Enter the second number: \n");
        fgets(buff2, sizeof(buff2), stdin); 
        send(sockfd, buff1, sizeof(buff1),0);  // Send the message to the server
        send(sockfd, buff2, sizeof(buff2),0);
        printf("Numbers sent to server!!\n");
        
        recv(sockfd,buff3,sizeof(buff3),0);

        // Read the server's response
        printf("From server: %s\n", buff3);  // Display the message received from the server

    // Close the connection
    close(sockfd);
    printf("Connection closed with server.\n");

}

