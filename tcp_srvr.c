#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h> // For strcmp

#define PORT 8080
#define SA struct sockaddr
#define MAX 1024

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    socklen_t len;
    char b1[MAX];
    char b2[MAX];
    char b3[MAX];
    int n1,n2,sum;

// Socket creation
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Socket creation failed!!\n");
        return -1;
    }
    else {
        printf("Socket created successfully\n");
    }

// Setting Socket options
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        printf("Error in setting socket options\n");
        close(sockfd); 
        return -1;
    }
    else {
        printf("Socket option SO_REUSEADDR set successfully\n");
    }

// Server address setup
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

// Binding the socket to the port
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed...\n");
        close(sockfd); 
        return -1;
    }
    else {
        printf("Socket successfully bound..\n");
    }

// Listen for incoming connections
    if (listen(sockfd, 1) != 0) {
        printf("Listen failed\n");
        close(sockfd); 
        return -1;
    }
    else {
        printf("Server listening...\n");
    }

// Accept a client connection
    len = sizeof(cli);
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0) {
        printf("Server accept failed...\n");
        close(sockfd); 
        return -1;
    }
    else {
        printf("Server accepted the client...\n");
    }

        // Read message from client
	bzero(b1,sizeof(b1));
	bzero(b2,sizeof(b2));
	bzero(b3,sizeof(b3));      
	recv(connfd, b1, sizeof(b1),0);
	recv(connfd, b2, sizeof(b2),0);
        
        n1 = atoi(b1);
        n2 = atoi(b2);
 	sum = n1 + n2;
 	
 	sprintf(b3,"%d",sum);
	printf("Sum of numbers sent from client : %d\n",sum);
	
        // Send message to client
        send(connfd, b3, sizeof(b3),0);  // Send the message to the client


    // Close the connection after exiting the loop
    close(connfd);
    printf("Connection closed with client.\n");

    // Close the server socket
    close(sockfd);
    printf("Server closed.\n");
}

