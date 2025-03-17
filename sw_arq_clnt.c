#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    int client_sock;
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);
    char buffer[1024];
    char message[1024];
    int seq = 0;

    srand(time(NULL)); // Seed for randomization

    // Create socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == -1) {
        perror("Error: Socket creation failed");
        return -1;
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Connect to the server
    if (connect(client_sock, (struct sockaddr *)&server_addr, server_len) == -1) {
        perror("Error: Connection failed");
        return -1;
    }

    // Set timeout for receiving data
    struct timeval tv;
    tv.tv_sec = 2; // 2 seconds timeout
    tv.tv_usec = 0;
    setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    printf("Client: Please enter the message to send:\n");
    fgets(message, sizeof(message), stdin);  // Read message from user

    // Send each character of the message with simulated packet loss
    for (int i = 0; i < strlen(message); i++) {
        char lastsent = message[i];

        // Simulate packet loss (50% chance)
        int loss = rand() % 2;

        if (loss == 0) {
            printf("Client: Simulating loss for character '%c'\n", lastsent);
            snprintf(buffer, sizeof(buffer), "%c", lastsent);
            send(client_sock, buffer, strlen(buffer), 0);  // Send the packet
            printf("Client: Resending '%c' due to packet loss\n", lastsent);
        } else {
            snprintf(buffer, sizeof(buffer), "%c", lastsent);
            send(client_sock, buffer, strlen(buffer), 0);  // Send the packet
            printf("Client: Sent '%c' to server\n", lastsent);
        }

        // Wait for acknowledgment
        while (1) {
            int received = recv(client_sock, buffer, sizeof(buffer), 0);

            if (received <= 0) {
                printf("Client: No ACK received for '%c'. Resending...\n", lastsent);
                send(client_sock, buffer, strlen(buffer), 0);  // Resend on timeout
            } else {
                buffer[received] = '\0';
                printf("Client: Received ACK: %s from server\n", buffer);
                break;
            }
        }
    }

    // Close the socket
    close(client_sock);
    return 0;
}

