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
    struct sockaddr_in servaddr, cli;
    socklen_t len;
    char buff[MAX];

// Socket creation
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Socket creation failed!!\n");
        return -1;
    } else {
        printf("Socket created successfully\n");
        printf("Sockfd = %d\n", sockfd);
    }

// Setting Socket options
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        printf("Error in setting socket options\n");
        close(sockfd); 
        return -1;
    } else {
        printf("Socket option SO_REUSEADDR set successfully\n");
    }

// Server address setup
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

// Binding the socket to the port
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed...\n");
        close(sockfd); 
        return -1;
    } else {
        printf("Socket successfully bound..\n");
    }

    printf("Server is listening...\n");
    
    while (1) {
        // Read message from client
        bzero(buff, sizeof(buff));
        len = sizeof(cli);
        recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&cli, &len); // Receive from client
        printf("From client: %s\n", buff);  

        // If the client sends "exit", break the loop
        if (strcmp(buff, "exit") == 0) {
            printf("Client has exited. Exiting server...\n");
            break;
        }

        // Send message to client
        bzero(buff, sizeof(buff));
        printf("Enter a message to send to client: \n");
        fgets(buff, sizeof(buff), stdin); // Using fgets to safely take input

        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&cli, len); // Send to client
        
        printf("Message sent to client: %s\n", buff);

        // If the server sends "exit", break the loop
        if (strcmp(buff, "exit\n") == 0) {
            printf("Server has exited. Closing connection...\n");
            break;
        }
    }

    // Close the server socket
    close(sockfd);
    printf("Server closed.\n");

    return 0;
}

