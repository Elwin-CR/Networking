#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<time.h>
#include<unistd.h>

#define WINDOW_SIZE 4
#define TIMEOUT 2

void send_packet(int sock, struct sockaddr_in *server_addr, socklen_t addr_len, char *packet, int seq_num) {
    packet[0] = seq_num;  // Store sequence number at the beginning of the packet
    sendto(sock, packet, strlen(packet) + 1, 0, (struct sockaddr *)server_addr, addr_len);  // Corrected packet size
    printf("Sent packet with sequence number: %d\n", seq_num);
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);
    char message[] = "Hello, this is a message to send using Go-Back-N ARQ.";
    char packet[1024];
    int seq_num = 0;
    int base = 0;
    int next_seq_num = 0;
    int ack_received[strlen(message)];  // Dynamically allocated for tracking ACKs
    memset(ack_received, 0, sizeof(ack_received));  // Initialize to 0

    srand(time(NULL));

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Socket creation failed...");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8094);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Send packets while we have data to send
    while (next_seq_num < base + WINDOW_SIZE && next_seq_num < strlen(message)) {
        snprintf(packet + 1, sizeof(packet) - 1, "%c", message[next_seq_num]);
        send_packet(sock, &server_addr, addr_len, packet, seq_num);
        next_seq_num++;
        seq_num++;
    }

    // Main loop to handle timeouts, ACK reception, and sliding window
    while (base < strlen(message)) {
        fd_set read_fds;
        struct timeval tv;
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(sock, &read_fds);

        int ret = select(sock + 1, &read_fds, NULL, NULL, &tv);
        if (ret > 0 && FD_ISSET(sock, &read_fds)) {
            int ack_num;
            int recv_len = recvfrom(sock, &ack_num, sizeof(ack_num), 0, NULL, NULL);
            if (recv_len > 0) {
                ack_num = ntohl(ack_num);
                printf("Received ACK for packet %d\n", ack_num);
                ack_received[ack_num] = 1;
            }
        }

        while (ack_received[base] == 1) {
            printf("Base moved to %d\n", base + 1);
            base++;
        }

        for (int i = base; i < next_seq_num && i < base + WINDOW_SIZE; i++) {
            if (ack_received[i] == 0) {
                snprintf(packet + 1, sizeof(packet) - 1, "%c", message[i]);
                send_packet(sock, &server_addr, addr_len, packet, i);
                printf("Resent packet %d\n", i);
            }
        }

        while (next_seq_num < base + WINDOW_SIZE && next_seq_num < strlen(message)) {
            snprintf(packet + 1, sizeof(packet) - 1, "%c", message[next_seq_num]);
            send_packet(sock, &server_addr, addr_len, packet, seq_num);
            next_seq_num++;
            seq_num++;
        }
    }

    close(sock);
    return 0;
}

