#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrlen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);
    client_fd = accept(server_fd, (struct sockaddr *)&clientAddr, (socklen_t *)&clientAddrlen);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int len = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (len > 0) {
            buffer[len] = '\0';  // Null-terminate the received data
            printf("Server received message \"%s\"\n", buffer);

            // Send acknowledgment back to the client
            char ack_message[BUFFER_SIZE];
            snprintf(ack_message, BUFFER_SIZE, "Server received message \"%s\"", buffer);
            send(client_fd, ack_message, strlen(ack_message), 0);
        } else if (len == 0) {
            printf("Client disconnected.\n");
            break;
        } else {
            perror("recv failed");
            break;
        }
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
