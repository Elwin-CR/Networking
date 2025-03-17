#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#define WINDOW_SIZE 4
#define BUFFER_SIZE 1024
#define TIMEOUT 2

// Function to process the received packet and send ACK
void process_packet(int sock, struct sockaddr_in *client_addr, socklen_t addr_len, char *packet, int seq_num) {
    int ack_num = seq_num;  // Acknowledge the received sequence number
    sendto(sock, &ack_num, sizeof(ack_num), 0, (struct sockaddr *)client_addr, addr_len);  // Send ACK to client
    printf("Server: Sent ACK for packet %d\n", ack_num);
}

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    srand(time(NULL));

    // Create a UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("Socket creation failed...\n");
        return -1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind the server socket
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Bind failed\n");
        return -1;
    }

    printf("Server waiting for packets on port 8094...\n");

    int expected_seq_num = 0;

    // Server loop to receive packets
    while (1) {
        // Receive a packet from the client
        int received = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (received <= 0) {
            printf("Reception failed\n");
            break;
        }

        int seq_num = buffer[0];  // The sequence number is at the start of the packet
        buffer[received] = '\0';   // Null-terminate the string for printing

        // Print the received packet data and sequence number
        printf("Received packet with sequence number %d: %s\n", seq_num, buffer + 1);

        // If the sequence number matches the expected one, acknowledge it
        if (seq_num == expected_seq_num) {
            process_packet(sock, &client_addr, addr_len, buffer, seq_num);
            expected_seq_num++;  // Expect the next sequence number
        } else {
            printf("Ignored packet with sequence number: %d\n", seq_num);
        }
    }

    // Close the socket before exiting
    close(sock);
    return 0;
}

