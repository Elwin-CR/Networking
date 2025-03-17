#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t server_len = sizeof(server_addr);
    socklen_t client_len = sizeof(client_addr);
    int seq = 0;

    srand(time(NULL)); // Seed for randomization

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Error: Socket creation failed");
        return -1;
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind the socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, server_len) == -1) {
        perror("Error: Binding failed");
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_sock, 1) == -1) {
        perror("Error: Listen failed");
        return -1;
    }
    printf("Server: Listening on port %d...\n", 8080);

    // Accept client connection
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock == -1) {
        perror("Error: Accept failed");
        return -1;
    }

    char buffer[1024];

    while (1) {
        // Receive data from client
        int received = recv(client_sock, buffer, sizeof(buffer), 0);
        if (received <= 0) {
            printf("Server: Connection closed or error occurred\n");
            break;
        }

        buffer[received] = '\0';
        printf("Server: Received message '%s' from client\n", buffer);

        // Simulate ACK loss (50% chance)
        int ack_loss = rand() % 2;
        if (ack_loss == 0) {
            printf("Server: Simulating ACK loss for sequence %d\n", seq);
            continue; // No ACK sent for this packet
        }

        // Simulate delayed ACK (50% chance)
        int delay = rand() % 2;
        if (delay == 1) {
            printf("Server: Simulating delayed ACK for sequence %d\n", seq);
            sleep(1);  // Simulate 1 second delay before sending ACK
        }

        snprintf(buffer, sizeof(buffer), "ACK %d", seq);
        send(client_sock, buffer, strlen(buffer), 0);  // Send acknowledgment
        printf("Server: Sent ACK %d to client\n", seq);

        // Toggle the sequence number for the next packet
        seq ^= 1;
    }

    // Close the sockets
    close(client_sock);
    close(server_sock);
    return 0;
}

